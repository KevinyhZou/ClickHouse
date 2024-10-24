#include <Functions/FunctionFactory.h>
#include <Functions/DateTimeTransforms.h>
#include <Functions/FunctionDateOrDateTimeToSomething.h>
#include <DataTypes/DataTypesNumber.h>


namespace DB
{

using FunctionToRelativeQuarterNum = FunctionDateOrDateTimeToSomething<DataTypeUInt32, ToRelativeQuarterNumImpl<ResultPrecision::Standard>>;

void registerFunctionToRelativeQuarterNum(FunctionFactory & factory)
{
    factory.registerFunction<FunctionToRelativeQuarterNum>();
}

}


