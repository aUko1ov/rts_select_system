
#pragma once

#define SIMPLE_ACTION_TO_UNIT_INTERFACE(ActionName) \
void ActionName() \
{ \
if (IUnitInterface) \
{ \
IUnitInterface->ActionName(); \
} \
}

#define PARAMETERIZED_ACTION_TO_UNIT_INTERFACE(ActionName, ParamType) \
void ActionName(ParamType Value) \
{ \
if (IUnitInterface) \
{ \
IUnitInterface->ActionName(Value); \
} \
}