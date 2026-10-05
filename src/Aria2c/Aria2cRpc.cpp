
#include "Aria2c/Aria2cRpc.h"

unsigned int rpcMethodToUnsignedInt(
    const RpcMethod method
) {
    return static_cast<unsigned int>(method);
};

QString rpcMethodEnumToRpcMethodName(
    const RpcMethod method
) {
    return rpcMethodNames[rpcMethodToUnsignedInt(method)];
};
