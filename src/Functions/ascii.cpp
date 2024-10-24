#include <DataTypes/DataTypeString.h>
#include <Functions/FunctionFactory.h>
#include <Functions/FunctionStringOrArrayToT.h>

namespace DB
{

namespace ErrorCodes
{
    extern const int ILLEGAL_TYPE_OF_ARGUMENT;
    extern const int NOT_IMPLEMENTED;
}

struct AsciiName
{
    static constexpr auto name = "ascii";
};


struct AsciiImpl
{
    static constexpr auto is_fixed_to_constant = false;
    using ReturnType = Int32;


    static void vector(const ColumnString::Chars & data, const ColumnString::Offsets & offsets, PaddedPODArray<ReturnType> & res)
    {
        size_t size = offsets.size();

        ColumnString::Offset prev_offset = 0;
        for (size_t i = 0; i < size; ++i)
        {
            res[i] = doAscii(data, prev_offset, offsets[i] - prev_offset - 1);
            prev_offset = offsets[i];
        }
    }

    [[noreturn]] static void vectorFixedToConstant(const ColumnString::Chars &  /*data*/, size_t  /*n*/, Int32 &  /*res*/)
    {
        throw Exception(ErrorCodes::NOT_IMPLEMENTED, "vectorFixedToConstant not implemented for function {}", AsciiName::name);
    }

    static void vectorFixedToVector(const ColumnString::Chars & data, size_t n, PaddedPODArray<ReturnType> & res)
    {
        size_t size = data.size() / n;

        for (size_t i = 0; i < size; ++i)
        {
            res[i] = doAscii(data, i * n, n);
        }
    }

    [[noreturn]] static void array(const ColumnString::Offsets & /*offsets*/, PaddedPODArray<ReturnType> & /*res*/)
    {
        throw Exception(ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT, "Cannot apply function {} to Array argument", AsciiName::name);
    }

    [[noreturn]] static void uuid(const ColumnUUID::Container & /*offsets*/, size_t /*n*/, PaddedPODArray<ReturnType> & /*res*/)
    {
        throw Exception(ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT, "Cannot apply function {} to UUID argument", AsciiName::name);
    }

private:
    static Int32 doAscii(const ColumnString::Chars & buf, size_t offset, size_t size)
    {
        return size ? static_cast<ReturnType>(buf[offset]) : 0;
    }
};

using FunctionAscii = FunctionStringOrArrayToT<AsciiImpl, AsciiName, AsciiImpl::ReturnType>;

void registerFunctionAscii(FunctionFactory & factory)
{
    factory.registerFunction<FunctionAscii>(FunctionFactory::CaseInsensitive);
}

}
