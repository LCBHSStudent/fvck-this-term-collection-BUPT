#ifndef MESSAGETYPEGLOBAL_H
#define MESSAGETYPEGLOBAL_H

enum MessageType: int {
    UserSignUpRequest = 0,
    UserSignUpResponse,
    
    UserLoginRequest,
    UserLoginResponse,
    
    UserInfoRequest,
    UserInfoResponse,
    
    UserPokemonDataRequest,
    UserPokemonDataResponse,
    
    PokemonDataRequest,
    PokemonDataResponse,
    
    OnlineUserListRequest,
    OnlineUserListResponse,
    
    BattleTurnInfo,
    BattleOperationInfo,
    BattleFinishInfo,
    
    BattleStartRequest,
    BattleStartResponse,
    
    ChooseTrophieRequest,
    ChooseTrophieResponse,
};

#endif // MESSAGETYPEGLOBAL_H
