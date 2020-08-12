#ifndef MESSAGETYPEGLOBAL_H
#define MESSAGETYPEGLOBAL_H

// ------ 供client与server共用的存储于报文头的数据类型定义
enum MessageType: int {
    UserSignUpRequest = 0,  // 注册请求
    UserSignUpResponse,     // 注册应答
    
    UserLoginRequest,       // 登录请求
    UserLoginResponse,      // 登录应答
    
    UserInfoRequest,        // 用户信息请求
    UserInfoResponse,       // 用户信息应答
    
    PokemonDataRequest,     // 宝可梦信息请求
    PokemonDataResponse,    // 宝可梦信息应答
    
    OnlineUserListRequest,  // 在线用户列表请求
    OnlineUserListResponse, // 在线用户列表应答
    
    BattleTurnInfo,         // 对战回合信息
    BattleGiveupInfo,       // 对战放弃信息
    BattleOperationInfo,    // 对战操作信息
    BattleFinishInfo,       // 对战结束信息
    
    BattleStartRequest,     // 开始对战请求
    BattleStartResponse,    // 开始对战应答
    
    BattleInviteRequest,    // 对战邀请请求
    BattleInviteResponse,   // 对战邀请应答
    
    TransferPokemonRequest, // 宝可梦所有权转移请求
    TransferPokemonResponse,// 宝可梦所有权转移应答
};

#endif // MESSAGETYPEGLOBAL_H
