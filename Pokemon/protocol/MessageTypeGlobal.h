#ifndef MESSAGETYPEGLOBAL_H
#define MESSAGETYPEGLOBAL_H

enum MessageType: int {
    UserSignUpRequest = 0,
    UserSignUpResponse,
    
    UserLoginRequest,
    UserLoginResponse,
    
    UserInfoRequest,
    UserInfoResponse,
    
    PokemonDataRequest,
    PokemonDataResponse,
    
    OnlineUserListRequest,
    OnlineUserListResponse,
    
    BattleTurnInfo,
    BattleGiveupInfo,
    BattleOperationInfo,
    BattleFinishInfo,
    
    BattleStartRequest,
    BattleStartResponse,
    
    BattleInviteRequest,
    BattleInviteResponse,
    
    ChooseTrophieRequest,
    ChooseTrophieResponse,
};

#endif // MESSAGETYPEGLOBAL_H
