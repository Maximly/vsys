/*++

Abstract:

    Test static ctors/dtors

Revision History:

    23/08/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once

#define CTORSEQ_START 100

class CtorTestBase
{
public:
    CtorTestBase() : base_seq_(global_seq_) {
        OnCtor("CtorTestBase");
    }
    virtual ~CtorTestBase() {
        OnDtor("CtorTestBase");
    }

protected:
    virtual int GetMySeq() const { return base_seq_; }
	void OnCtor(const char* ctor) {
        DbgPrint(("\t%-24s::Ctor() sequence # %i, this = %px", ctor, GetMySeq() - CTORSEQ_START + 1, this));
        global_seq_++;
    }
    void OnDtor(const char* dtor) {
        global_seq_--;
        DbgPrint(("\t%-24s::Dtor() sequence # %i (%s) this = %px", dtor, GetMySeq() - CTORSEQ_START + 1, GetMySeq() == global_seq_ ? "ok" : "error", this));
    }

    static int global_seq_;

private:
    int base_seq_;
};


class CtorTest : public CtorTestBase
{
public:
    CtorTest() : seq_(global_seq_) {
        OnCtor("CtorTestBase::CtorTest");
    }
    virtual ~CtorTest() {
        OnDtor("CtorTestBase::CtorTest");
    }

protected:
    virtual int GetMySeq() const { return seq_; }

private:
    int seq_;
};

