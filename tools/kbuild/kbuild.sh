#!/bin/bash
###############################################################################
#
# Abstract:
#   Collect linux kernel build parameters
#     Usage:
#       kbuild.sh [kernel headers path]
#
# Revision History:
#   27/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################

# Setting up output beautifier
bold=$(tput bold)
red=$(tput setaf 1)
normal=$(tput sgr0)

# Working in script directory
cd $(dirname "$0")

# Kernel headers path
kernel_headers=$1
if [[ ! -f "$kernel_headers" ]]; then
    kernel_headers=$(echo -e "/usr/src/linux-headers-$(uname -r)")
fi

# Generated files
cmake_generated_dir=../cmake/generated
kbuild_cmake=$cmake_generated_dir/kbuild.cmake
kbuild_cmake_tmp=${kbuild_cmake}.tmp
kbuild_cmake_c_tmp=${kbuild_cmake}_c.tmp
kbuild_cmake_cxx_tmp=${kbuild_cmake}_cxx.tmp
inc_generated_dir=../../inc/linux/kernel/generated
src_generated_dir=../../src/linux/kernel/generated
precompiler_h=$inc_generated_dir/precompiler.h
mkdir -p $src_generated_dir
module_cpp=$(cd "$src_generated_dir" && pwd)/module.cpp

# Kbuild cleanup
cleanup="rm -rf *.o *~ core .depend .*.cmd *.ko *.mod *.mod.c .tmp_versions *.order *.symvers $kbuild_cmake_tmp $kbuild_cmake_c_tmp $kbuild_cmake_cxx_tmp"


###############################################################################
#
#   Sed scripts for make outputs
#
###############################################################################
# Sed script for parsing compiler parameters
sed_compiler_options='
    /^cmd_.*$/ {
        s/^\(.*\)-Wp,[^ ]*\( .*\)$/\1\2/;
        s/^[^:]*:= *\([^ ]\+\) [^-]*\(-.*\) -o.*$/\2/;
        s/-I./-I${KERNEL_HEADERS}/g;
        s/'"'"'"/"/g;
        s/"'"'"'/"/g;
        s/ \+-/\n-/g;
        s/kbuild/${PROJECT_NAME}/g;
        p;
    }'

# Sed scripts for updating compiler parameters for C++
cxx_remove_options='
    -nostdinc
    -Werror=strict-prototypes
    -Werror=implicit-function-declaration
    -Werror=implicit-int
    -Wdeclaration-after-statement
    -Wno-pointer-sign
    -Werror=incompatible-pointer-types
    -Werror=designated-init
    '
cxx_add_options='
    -Wno-literal-suffix
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -fpermissive
    -fno-exceptions
    -include base.h
    -fmax-errors=5
    '
sed_options_list_to_string='s/ \+/\\|/g'
cxx_remove_options_list=$(echo $cxx_remove_options | sed -e "$sed_options_list_to_string")
sed_cxx_compiler_options='
    s/^\(-std=gnu\)11\(.*\)$/\1++14\2/p;
    /^\(-std=gnu\|'$cxx_remove_options_list'\).*$/! p;
    '

# Sed script for parsing linker parameters
sed_linker_options='
    /^[^:]*:= .*-o.*$/ {
        s/^[^:]*:= *\([^ ].\) [^-]*\(-.*\) -o.*$/\2/;
        s/ \+/,/g;
        s/^\(.*-T,\)\(.*\)$/\1${KERNEL_HEADERS}\/\2/;
        s/^\(.*\)$/target_link_options(${PROJECT_NAME} BEFORE PRIVATE -r LINKER:\1)/p;
    }
    '

# Sed script for moving -include options to kernel/linux/_kernel.h header to work around cmake' options de-dup feature
sed_kernel_h='s/^-include.*\(linux.*\)$/#include <\1>/p'
sed_remove_includes='/^-include.*$/! p'

# Sed script for fixing designator order for field ‘module::arch'
sed_fix_module_arch='
    /^#ifdef CONFIG_MODULE_UNLOAD$/ {
        N; N; N; N;
        s/^\(.*\n\)\([ 	]*\.arch.*\n\)\(.*\)$/\2\1\3/
    }
    '


###############################################################################
#
#   Make kbuild stub
#
###############################################################################
echo $bold'VSYS: Building empty Linux kernel module'$normal
make
make_exit=$?
if [ "$make_exit" != "0" ] ; then
    echo ${bold}${red}VSYS: Error $make_exit building kbuild stub${normal}
    exit $make_exit
fi


###############################################################################
#
#   Process make outputs
#
###############################################################################
echo $bold'VSYS: Detecting compiler parameters'$normal
mkdir -p $cmake_generated_dir
sed -n -e "$sed_compiler_options" <.kbuild.o.cmd >$kbuild_cmake_tmp
mkdir -p $inc_generated_dir
echo '//  Autogenerated header with detected precompiler includes' >$precompiler_h
sed -n -e "$sed_kernel_h" <$kbuild_cmake_tmp >>$precompiler_h
sed -n -e "$sed_remove_includes" <$kbuild_cmake_tmp >$kbuild_cmake_c_tmp
rm $kbuild_cmake_tmp
sed -n -e "$sed_cxx_compiler_options" <$kbuild_cmake_c_tmp >$kbuild_cmake_cxx_tmp
echo $cxx_add_options >>$kbuild_cmake_cxx_tmp
echo "# Autogenerated Linux kernel build cmake file" >$kbuild_cmake
echo 'target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:' >>$kbuild_cmake
sed -e "s/^\(.*\)$/    \1/" <$kbuild_cmake_cxx_tmp >>$kbuild_cmake
echo '>)' >>$kbuild_cmake
echo -e "\ntarget_compile_options(\${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:C>:" >>$kbuild_cmake
sed -e "s/^\(.*\)$/    \1/" <$kbuild_cmake_c_tmp >>$kbuild_cmake
echo '>)' >>$kbuild_cmake
echo -e "\ntarget_sources(\${PROJECT_NAME} PRIVATE ${module_cpp})\n" >>$kbuild_cmake

echo $bold'VSYS: Detecting linker parameters'$normal
sed -n -e "$sed_linker_options" <.kbuild.ko.cmd >>$kbuild_cmake

echo $bold'VSYS: Generating module.cpp'$normal
echo -e "#include \"base.h\"\n\nextern \"C\" {\n" >$module_cpp
sed -e "$sed_fix_module_arch" <kbuild.mod.c >>$module_cpp
echo -e "\n};" >>$module_cpp


###############################################################################
#
#   Patch kernel headers
#
###############################################################################
rm -f $kbuild_cmake_tmp

add_header_path() {
    path=$(dirname $1)
    while [ "$path" != "" ]; do
        echo '${CMAKE_SOURCE_DIR}/inc/linux/kernel/generated/'$path >>$kbuild_cmake_tmp
        path1=$path
        path=${path%/*}
        if [ "$path" = "$path1" ]; then
            path="";
        fi
    done
}

#   Headers processing scripts
sed_rename_new='s/\([[() *\t&>]\)\(new\|private\|protected\|class\|namespace\)\([][ \t);,]\|->\)/\1\21\3/g'
sed_init_union='s/^\(.*union \+{.*}[^;=]*\)\(;.*\)/\1 = {}\2/g'
sed_remove_void='s/^\(.*\)((void)0)\(.*\)$/\1(0)\2/'
sed_rename_const_void='s/^\(.*\)const void\(.*__nosave_begin.*\)$/\1const int\2/'
sed_disable_build_bug='s/^\(.*define BUILD_BUG_ON_ZERO(e)[^(]\+(\)(.*$/\10)/'
sed_seqprop='/^#define __seqprop.*$/,/^.*__seqprop_case.*))$/d; /^#define seqprop_ptr.*$/ s/^\(.*\)$/#include <linux\/kernel\/seqprop.h>\n\1/';
sed_hrtimer='s/^\(.*enum \)\(hrtimer_restart\|aarch64_insn_encoding_class\)\([^{]*{.*\)$/\1\2 : int\3/'
sed_log2_constexpr='s/^\(.*\)\(int\|bool\|long\)\( \)\(__ilog2_u32\|__ilog2_u64\|is_power_of_2\|__order_base_2\|__bits_per\)\((.*\)$/\1constexpr \2\3\4\5/';
sed_fls_constexpr='s/^\(.*\)\(int\|long\)\( *\)\(fls\|fls64\|__fls\)\((.*\)$/\1constexpr \2\3\4\5/';
sed_rcu_init='s/^\(.*define RCU_INITIALIZER(v)\).*$/\1 (v)/';
sed_rcu_typeof='/typeof(\*p)/ s/\*//g';
sed_page_folio='/^#define page_folio(.*$/ { N; N; s/^.*$/#define page_folio vsys_page_folio/; }';
sed_invalid_uid='/^#define INVALID_\([GU]ID\|PROJID\).*$/ s/-1/UINT_MAX/'
sed_mov_ki_pos='/^[^.]*\.ki_filp =.*$/ { N; N; N; N; s/^\([^.]*\.ki_filp =.*,\n\)\([^.]*\.ki_flags =.*\n\)\([^.]*\.ki_pos =[^\n]*\n\)\(.*\)$/\1\3\2\4/; }';
sed_lt_from_rb='/^__lt_from_rb(struct rb_node \*node, int idx).*$/ { N; N; N; s/^\([^\{]*{\)[^\}]*\(\}.*\)$/\1\n\treturn (struct latch_tree_node *)((char*)container_of(node, struct latch_tree_node, node[0]) - sizeof(*node) * idx);\n\2/; }';
headers_list=(
    "arch/arm64/include/asm/atomic_ll_sc.h"         "$sed_rename_new"
    "arch/arm64/include/asm/atomic_lse.h"           "$sed_rename_new"
    "arch/arm64/include/asm/barrier.h"              "$sed_init_union"
    "arch/arm64/include/asm/cmpxchg.h"              "$sed_rename_new"
    "arch/arm64/include/asm/insn.h"                 "$sed_hrtimer"
    "include/asm-generic/sections.h"                "$sed_rename_const_void"
    "include/asm-generic/bitops/builtin-fls.h"      "$sed_fls_constexpr"
    "include/asm-generic/bitops/builtin-__fls.h"    "$sed_fls_constexpr"
    "include/asm-generic/bitops/fls64.h"            "$sed_fls_constexpr"
    "include/linux/atomic/atomic-arch-fallback.h"   "$sed_rename_new"
    "include/linux/atomic/atomic-long.h"            "$sed_rename_new"
    "include/linux/atomic/atomic-instrumented.h"    "$sed_rename_new"
    "include/linux/bitmap.h"                        "$sed_rename_new"
    "include/linux/build_bug.h"                     "$sed_disable_build_bug"
    "include/linux/fs.h"                            "$sed_rename_new; $sed_mov_ki_pos"
    "include/linux/hrtimer.h"                       "$sed_hrtimer"
    "include/linux/ioasid.h"                        "$sed_rename_new"
    "include/linux/ioprio.h"                        "$sed_rename_new"
    "include/linux/kobject.h"                       "$sed_rename_new"
    "include/linux/list.h"                          "$sed_rename_new"
    "include/linux/llist.h"                         "$sed_rename_new"
    "include/linux/log2.h"                          "$sed_log2_constexpr"
    "include/linux/math.h"                          "$sed_remove_void"
    "include/linux/mm_types.h"                      "$sed_rename_new"
    "include/linux/mmzone.h"                        "$sed_rename_new"
    "include/linux/page-flags.h"                    "$sed_page_folio"
    "include/linux/pid.h"                           "$sed_rename_new"
    "include/linux/projid.h"                        "$sed_invalid_uid"
    "include/linux/rbtree.h"                        "$sed_rename_new"
    "include/linux/rbtree_latch.h"                  "$sed_lt_from_rb"
    "include/linux/rculist.h"                       "$sed_rename_new"
    "include/linux/rcupdate.h"                      "$sed_rename_new; $sed_rcu_init; $sed_rcu_typeof"
    "include/linux/seqlock.h"                       "$sed_seqprop"
    "include/linux/string.h"                        "$sed_rename_new"
    "include/linux/sysfs.h"                         "$sed_rename_new"
    "include/linux/uidgid.h"                        "$sed_invalid_uid"
    "include/linux/umh.h"                           "$sed_rename_new"
    "include/linux/wait.h"                          "$sed_rename_new"
    )

headers_size=${#headers_list[@]}
echo $bold"VSYS: Patching $(($headers_size / 2)) headers"$normal

# Detect current platform
platform=$(uname -m)
platform=${platform,,}
if [ "$platform" = "aarch64" ]; then
    platform="arm64"
fi

i=0
while [ $i -lt $headers_size ]; do
    header=${headers_list[$i]}
    script=${headers_list[$i + 1]}
    i=$(($i + 2))
    if [[ -f "$kernel_headers/$header" ]]; then
        arch_header=$(echo "$header" | sed -n -e "s/^arch\/\([^\/]\+\)\/.*$/\1/p")
        if [ "$arch_header" != "" ] && [ "$arch_header" != "$platform" ]; then
            echo "  $header is skipped (not for this platform)"
            continue
        fi
        echo "  $header"
        mkdir -p $(dirname $inc_generated_dir/$header)
        add_header_path $(dirname $header)
        sed -e "$script" <$kernel_headers/$header >$inc_generated_dir/$header
    else
        echo "  $header is skipped"
    fi
done


#   Saving generated paths
echo '
#   Add generated and patched Linux kernel headers
target_include_directories(${PROJECT_NAME} BEFORE PRIVATE
    ${CMAKE_SOURCE_DIR}/inc/linux/kernel/generated' >>$kbuild_cmake
if [[ -f "$kbuild_cmake_tmp" ]]; then
sed -e "s/^.*generated\/include\/linux.*$//; s/^\(.\+\)$/    \1/" <$kbuild_cmake_tmp | sort -u | grep -v '^$' >>$kbuild_cmake
fi
echo ')' >>$kbuild_cmake


###############################################################################
#
#   Cleaning up
#
###############################################################################
echo $bold'VSYS: Cleaning up'$normal
$cleanup
