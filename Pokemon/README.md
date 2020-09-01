# 基于QtTcpSocket protobuf c++17 实现的C/S模式宝可梦在线对战游戏系统
## 目录
1. 开发环境及配置说明
2. 系统设计介绍
3. 关于【运行】以及【自定义内容】（自定义宝可梦属性 & 自定义技能）的说明
4. stage说明
5. 实现效果预览

## 1.开发环境及配置说明
&emsp;&emsp;开发环境：**windows10 v2004** & **Qt version 5.14.1** & **MSVC version 1919**  
&emsp;&emsp;其他依赖：**android ndk-r21b** & **mysql-8.0.20 x64** & **protoc 3.12.0 x64** & **protobuf 3.12.4**
  
&emsp;&emsp;配置说明：本项目将client与server置于一个pri内，编译时需要的protobuf 依赖，其lib和header分别位于/Pokemon（项目根目录）下的/lib 与 /protobuf_headers 内。检查依赖，配置环境并选择编译kit为msvc2019后即可一次性完成client与server端的构建。
  
&emsp;&emsp;于windows环境下运行时，除了配置Qt项目本身所需要的运行时环境，还需要将 /bin 内的dll文件拷贝到项目的运行目录下。（**release版的运行目录下直接复制生成的libprotobuf.dll，debug版的运行目录下复制debug生成版，去掉文件名最后的d**）
  
&emsp;&emsp;关于网络环境的配置，修改**client端src/main.cpp**内的**ClientBackend构造参数字符串**为server端ipv4地址，在保证1919端口开放的条件下即可正常运行，若无法开放1919端口，则需要修改server与client的**NetworkHelper静态类变量-port**为指定的端口号。

## 2.系统设计介绍
### 总体 (根据原实践题目要求，分为三个题目来进行介绍)
1. 宝可梦（精灵）类的设计要求  
&emsp;&emsp;·设计宠物小精灵的类，精灵的属性包括种类（力量型：高攻击； 肉盾型：高生命值； 防御型：高防御； 敏捷型：低攻击间隔，共四种）、名字、等级、经验值、攻击力、防御力、生命值、速度等。  
&emsp;&emsp;·每个精灵初始等级为1，满级15级，当精灵升级的时候，类型对应的属性值有概率额外增加。（如：力量型额外增加攻击力——ATK）  
&emsp;&emsp;·每种类型的精灵有自己攻击方式的独特性。（如耐久型精灵可以概率增幅获得的Buff效果和Buff持续时间）  
根据以上要求所设计出的精灵类位于PokemonServer/src/Pokemons内，是以PokemonBase为基类，HighAtkPkm（高攻击型）、HighHpPkm（高耐久型）、HighDefPkm（高防御型）、HighSpdPkm（高速度型）为派生的类族。  
2. 用户注册与平台登录设计要求  
&emsp;&emsp;·每个用户需要注册一个账号，用户名全局唯一，不能有任何两个用户名相同，且注册失败时有场景的相关反馈。  
&emsp;&emsp;·实现注册、登录、登出功能，采用C/S模式，利用socket进行通信，服务端采用mysql server保存所有用户的信息。  
&emsp;&emsp;·每个用户拥有：用户名、密码、拥有的精灵、胜场数、总场数几个属性，注册成功时，系统自动随机分发三个1级精灵给用户。  
&emsp;&emsp;·用户可以查看所有成功注册用户拥有的精灵，也可以查看当前在线的用户列表。  
3. 游戏对战的设计要求  
&emsp;&emsp;·已经登录的在线用户可以和服务器进行虚拟决斗，决斗分两种：升级赛和决斗赛，两种比赛都能增长宠物经验值。服务器上有一个虚拟精灵的列表，用户可以挑选其中任意一个进行比赛（升级赛或者决斗赛）。另外决斗赛中用户胜出可以直接获得该战胜的的精灵，失败则系统从用户的精灵中随机选三个（不够三个精灵的情况就选择他所有的精灵），然后由用户选一个送出。  
&emsp;&emsp;·升级赛只是用来获取经验，胜利与失败均不计入场数 & 决斗赛则会获得/失去精灵，并且计入胜场胜率。  
&emsp;&emsp;·用户如果没有精灵（比如总是失败，已经全部送出去），则系统会随机放给给他一个初级精灵。  
&emsp;&emsp;·不同类型的精灵拥有不同类型的随机攻击时触发效果，并且技能效果也存在一定的随机性，保证了战斗结果一定程度上的的不确定性。  
&emsp;&emsp;·用户增加新属性，为宠物个数徽章（金银铜）和高级宠物徽章（金银铜），分别根据拥有的宠物个数的多少和拥有高级宠物（15级）个数的多少颁发。  
&emsp;&emsp;·用户可以查看某个用户的胜率、徽章等。  

### 全局组件
&emsp;&emsp;本项目的全局组件设计主要包括网络通信协议（报文格式，消息类型定义，protobuf message定义等）、宝可梦信息，对战信息，用户信息的定义。  
#### 1. &nbsp;/protocol/目录下存放的是C/S对网络消息类型定义枚举 **MessageTypeGlobal.h** 以及proto文件的编写。proto文件内容包括战斗相关协议 **BattleProtocol.proto** 和用户相关协议 **UserProtocol.proto**，在client与server之间通信的主要消息格式就依赖于这些文件的定义。
#### 2. &nbsp;数据库表的设计  
"user_list" 用户除精灵以外信息的存储表  
|  USERNAME   |  PASSWORD  |  TOTAL_BATTLE_TIME  |  WINNER_TIME  |
|  ----       |  ----      |  ----               |  ----         |
|  用户名      |  登录密码  |  决斗对战场数        | 决斗战胜场数   |
|  string     |  string    |  uint               | uint          |
|  UNIQUE     |            |                     |               |
  
    
"pokemon_info" 系统内精灵信息  
|  PKM_ID   |  PKM_NAME  |  PKM_TYPE  |  PKM_ATTR  |  ATK_INC | DEF_INC | HP_INC | SPD_INC | SKILL_1(2,3,4) | DESCRIPTION |
|  ----     |  ----      |  ----      |  ----      |  ----    |  ----   |  ----  |  ----   |  ----          |  ----      | 
|  精灵种族ID  |  精灵中文名称  |  精灵类型   | 精灵属性 | 攻击力成长值 | 防御力成长值 | 耐久度成长值 | 速度成长值 | 技能1~4名称（有四个字段） | 精灵描述 |
|  int      |  string    |  ENUM(int) | ENUM(int)  | int      | int     | int    | int     | string         | string     |
|  UNIQUE PRIMARY KEY    |   |        |            |          |         |        |         |                |            |
  
    
"skill_list" 记录技能描述、别名、中文名称  
| NAME | ALIAS | DESCRIPTION |
| ---- | ----  | ----        |
| 技能名 (英文 系统用) | 别名(中文) | 技能描述文本 |
| string | string | string |

   
   
"user_【后跟UserName】" 记录某用户精灵信息  
| PKM_ID | PKM_TYPEID | PKM_LEVEL | PKM_EXP | PKM_ATK | PKM_DEF | PKM_HP | PKM_SPD |
| ----   | ----       | ----      | ----    | ----    | ----    | ----   | ----    |
|持有编号|种族编号|等级|经验|攻击力|防御力|耐久度|速度|
|int|int|int|int|int|int|int|int|

#### 3. 全局socket通信设计
为了防止tcp socket的粘包问题，本项目采用了极其简陋的自定义协议设计，报文格式如下  

|&emsp;ByteLength(4 bytes)&emsp;|&emsp;MessageType(4 bytes)&emsp;|&emsp;protobuf message&emsp;|
由于client送往server的报文一般不存在分片问题，所以server端的接收socket可以不做合并处理，为了防止client一次不能接收到server送来的全部数据，双方接收回调的设计大致如下  
  
&emsp;&emsp;**server (NetworkHelper)**
```c++
/**
 * @method  socket有数据可读
 * @param   {void}
 * @return  {void}
 */
void NetworkHelper::slotReadClient() {
    QTcpSocket* client = reinterpret_cast<QTcpSocket*>(sender());
//  先拿到数据包头的大小
    while (client->bytesAvailable() >= sizeof(uint32)) {
        char lengthArr[sizeof(uint32)] = {0};
        client->read(lengthArr, sizeof(uint32));
        auto length = *reinterpret_cast<uint32*>(lengthArr);
        if (length < 0) {
            client->readAll();
//            throw std::runtime_error("")
        } else {
            QByteArray data = client->read(length);
            emit sigGetMessage(client, data);
        }
    }
}
```

&emsp;&emsp;**client (NetworkHelper)**
```c++
/**
 * @method  socket有数据可读
 * @param   {void}
 * @return  {void}
 */
void NetworkHelper::readyRead() {
    while (m_socket->bytesAvailable() >= sizeof(uint32)) {
        // 是否完成了完成数据包的读取
        static bool flag = true;    
        static uint32 length = 0;
        static char lengthArr[sizeof(uint32)] = {0};
        if (flag) {
            m_socket->read(lengthArr, sizeof(uint32));
            length = *reinterpret_cast<uint32*>(lengthArr);
        }
        if (length <= 0) {
            qDebug () << "coco";
            m_socket->readAll();
            flag = true;
//            throw std::runtime_error("")
        } else {
            if (length <= m_socket->bytesAvailable()) {
                flag = true;
                QByteArray data = m_socket->read(length);
                emit sigServerMessage(data);
            } else {
                flag = false;
                return;
            }
        }
    }
}
```


### Server端组件
&emsp;&emsp;Server端过滤后的结构如下  
————/include  
————————PreCompile.h（预编译头）  
————————Reflect.hpp（简陋反射辅助类）  
————/src  
————————/BattleField（对战管理类，处理对战操作&流程&结果）  
————————————BattleField.h  
————————————BattleField.cpp  
————————/NetworkHelper（网络辅助类，分拣信息给backend，管理client连接）  
————————————/NetworkHelper.h  
————————————/NetworkHelper.cpp  
————————/Pokemons（精灵类设计）  
————————————/PokemonBase（基类设计 & 相关辅助类）  
————————————————PokemonBase.h/cpp（精灵基类声明 & 定义）  
————————————————PokemonFactory.h/cpp（精灵工厂类声明 & 定义）  
————————————————PokemonSkill.h/cpp（精灵技能类声明 & 定义 & 技能函数编写）  
————————————VariousPokemon.cpp（四类派生类定义）  
————————————VariousPokemon.h（四类派生类声明）  
————————/ServerBackend  
————————————ServerBackend.h/cpp（server主要的事务处理类，存储client(用户)信息）  
————————/StorageHelper  
————————————StorageHelper.h/cpp（提供单例模式的mysql接口 & 比较方便的query exec方法）  
————————/User  
————————————User.h/cpp（用户类声明 & 定义）  
————PreCompile.cpp（结合预编译头）  
————UnitTest.cpp（测试类，几乎空白?！）  
————UnitTest.h  
————main.cpp（程序入口位点）  

### Client端组件

————/include  
————————PreCompile.h（预编译头）  
————/qml  
————————/components（弹窗、UI BASE、自定义控件）  
————————/pages（主页面、战斗页面、登录页面）  
————————/main.qml（qml入口，提供root context）  
————/res（ui、精灵、音频资源文件）  
————/src  
————————/ClientBackend（客户端后台类，主要负责消息收发，与前端交互）  
————————————/NetWorkHelper（网络辅助类，包含与服务器的连接管理，简易重连机制，简陋的状态检测）  
————————————————【连notify都没怎么写的屑程序】  
————————————ClientBackend.h/cpp  （后台类声明 & 设计 & 定义）  
————PreCompile.cpp（结合预编译头）  
————main.cpp（程序入口位点）  

## 3.关于【运行】以及【自定义内容】（自定义宝可梦 & 自定义技能）的说明
### &emsp;1. Mysql的默认口令为root@password，目标db名为pokemon，可于**PokemonServer/StorageHelper的构造函数内**修改  
### &emsp;2. 首次运行后，程序会于目标数据库内建立四个表：skill_list、pokemon_info、user_list、user__server，具体参照“2——全局组件——表设计”内的说明  
&emsp;&emsp;要添加新精灵时，参照表设计内填入精灵信息，并在client内放入对应种族id格式的小图标与包含正面与北面的对战用动态图片  
&emsp;&emsp;要添加服务器精灵时，按照格式填写系统内实际存在的精灵即可  
&emsp;&emsp;要添加技能时，参照内的格式填入技能信息，并且在**Server/PokemonSkill**中用技能英文名注册并实现，大致步骤如下：
```c++
// IN POKEMON_SKILL_H
SKILL_FUNC_DEF(GrassyTerrain/*替换为技能名*/);

// IN POKEMON_SKILL_CPP
REGISTER_SKILL(DragonPulse/*替换为技能名*/) {
    ATK_DEBUG(龙之波动);
    AttackResult result;
    /*
        AttackResult内容，buff id参照PokemonBase内的enum
        Buff selfBuf = {};
        Buff destBuf = {};
        int  selfDeltaHp = 0;
        int  destDeltaHp = 0;
    */
    
    // 使用DAMAGE(技能威力)来设置伤害
    result.destDeltaHp = DAMAGE(85);    
    
    // 使用嵌套JUDGE_OZ(成功率)来设置随机效果
    JUDGE_OZ(80) {
        result.destBuf.buffId  = PokemonBase::BuffType::DEF_DOWN_M;
        result.destBuf.turnCnt = 2;
    }
    
    // 使用user指针与dest指针访问使用者 & 目标宝可梦的属性信息，最好不要进行直接修改
    double rate = 1.0f + 
        static_cast<double>(user->m_HP - user->m_curHP) / 
        static_cast<double>(user->m_HP);


    return result;
}
```
## 4. stage说明
### &emsp;&emsp;stage1
>&emsp;&emsp;小精灵类设计时按照一个基类（PokemonBase，存储基本信息并定义虚函数接口），四种派生（HighAtkPkm、HighDefPkm、HighSpdPkm、HighHpPkm）实现了四种类别的精灵，其中派生类对于攻击函数（attack）、升级函数（levelup）根据不同精灵特性进行了重载，大致代码如下：
  
部分基类头文件
```cpp
/**
 * @brief The PokemonBase class
 */
class PokemonBase {
    friend class PokemonSkill;
public:     // 构造宝可梦类
		 PokemonBase(DEF_ARGS_LIST);
	virtual // 析构宝可梦类
		~PokemonBase() = default;

public RESOURCE:
    /**
     * @brief The PokemonType enum
     */
	enum PokemonType {
		HIGH_ATTACK = 0,
		HIGH_HITPOINT,
		HIGH_DEFENSE,
		HIGH_SPEED,
		UNDEFINED_TYPE
	};
    
    /* ...... */

    /**
     * @brief The PokemonAttribute enum
     */
    enum PokemonAttribute {
        FIRE = 0,
        WATER,
        GRASS,
        UNDEFINED_ATTR
    };    
	static const int        /*最大等级*/    
        MAX_LEVEL = 15;
    static int              /*升级经验*/
        LEVEL_UP_EXP[MAX_LEVEL-1];
    static PropertyCombo    /*初始属性值（废弃）*/
        INITIAL_PROPERTY;
    
public FUNCTION:
	void   /*经验值变化*/
		gainExperience(int exp);
    
    // ------ property属性组
    PROPERTY(int, id)
    PROPERTY(int, exp)
    PROPERTY(int, level)
    PROPERTY(int, typeID)
    PROPERTY(int, ATK)
    PROPERTY(int, curATK)
    PROPERTY(int, HP)
    PROPERTY(int, curHP)
    PROPERTY(int, DEF)
    PROPERTY(int, curDEF)
    PROPERTY(int, SPD)
    PROPERTY(int, curSPD)
    
    PROPERTY(PokemonType, pkmType)
    PROPERTY(PokemonAttribute, pkmAttr)
    
    PROPERTY(QString, name)
    PROPERTY(QString, curUser)
    PROPERTY(QString, desc)
    
public FUNCTION:
	virtual AttackResult    /*返回AttackResult的攻击函数*/
		attack(
                  PokemonBase&    target,   // 对方宝可梦引用
		    const QString&        skillName // 技能名
		) = 0;
	virtual void            /*升级时调用*/
		levelUp() = 0;
	void                    /*为技能槽设置技能*/
        setSkill(int slot, const QString& name) {
            if (slot < 4 && slot >= 0)    
                m_skills[slot] = name;    
        }
    QString                 /*获取技能槽内技能*/
        getSkill(int slot) const {
            if (slot < 4 && slot >= 0) { 
                return m_skills[slot];
            } else {
                return QString();
            }
        }
    void                    /*打印宝可梦状态信息*/
        printStatus() const;    
    void                    /*存储宝可梦信息到数据库*/
        save2LocalStorage() const;

protected RESOURCE:
	QString     /*宝可梦名称*/
		m_name  = {};
    QString     /*当前所有者用户名*/
        m_curUser = {};
    QString     /*宝可梦描述*/
        m_desc  = {};
    
    int         /*宝可梦持有ID*/
        m_id    = 0;
    int         /*宝可梦等级*/
		m_level = 0;
	int         /*宝可梦当前经验值*/
		m_exp	= 0;
    int         /*宝可梦种族id*/
        m_typeID = 0;

	int         /*宝可梦攻击力 & 当前攻击力*/
		m_ATK  = 0, m_curATK  = 0;
	int         /*宝可梦HP & 当前HP*/
		m_HP   = 0, m_curHP   = 0;
	int         /*宝可梦防御力 & 当前防御力*/
		m_DEF  = 0, m_curDEF  = 0;
	int         /*宝可梦速度 & 当前速度*/
		m_SPD = 0, m_curSPD = 0;
    
	PokemonType         /*宝可梦类型*/
		m_pkmType  = UNDEFINED_TYPE;
    PokemonAttribute    /*宝可梦属性*/
        m_pkmAttr  = UNDEFINED_ATTR;
    QVector<QString>    /*宝可梦技能槽*/
        m_skills   = {};
};

```
高攻击型派生类定义及重载函数实现
```cpp
/**
 * @brief The HighAtkPkm class
 */
class HighAtkPkm: public PokemonBase {
    friend class PokemonSkill;
public:	
		HighAtkPkm(DEF_ARGS_LIST);
	virtual
		~HighAtkPkm() = default;
public FUNCTION:	
	AttackResult
		attack(
            PokemonBase&    target,
            const QString&  skillName
		) override;
	void
		levelUp() override;
};

//----------------------ATK REFERED------------------------//
HighAtkPkm::HighAtkPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_ATTACK)) {}

// ------重写攻击型宝可梦的attack函数
// ------主要特性为有概率增幅伤害，并且无视防御型宝可梦的判定效果
/**
 * @brief HighAtkPkm::attack
 * @param target
 * @param skillName
 * @return {AttackResult} 攻击结果
 */
AttackResult HighAtkPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    AttackResult&& result = PokemonSkill::useSkillByName(skillName, this, &target);
    
    JUDGE_OZ(20) {
        if(result.destDeltaHp)
            result.destDeltaHp *= 1.2;
    }
    
    return result;
}

// ------重写攻击型宝可梦的升级函数
// ------主要特性为升级时概率性额外增加1点ATK
/**
 * @brief HighAtkPkm::levelUp
 */
void HighAtkPkm::levelUp() {
    LEVELUP_PREFIX
    JUDGE_OZ(20) {
        ATK_INC++;
    }
    LEVELUP_POSTFIX
}
```
>其中，每个精灵拥有四种技能，通过字符串反射调用静态函数即刻触发技能效果（产生buff，造成伤害，恢复体力等
  
技能类头文件部分代码
```cpp
/**
 * @brief The PokemonSkill class
 */
class PokemonSkill {
public:
    using SkillFunc = AttackResult(/*PokemonSkill::*/*)(PokemonBase*, PokemonBase*);
public FUNCTION:
    PokemonSkill()  = delete;
    ~PokemonSkill() = default;
    
    /**
     * @brief useSkillByName
     *        利用字符串调用函数
     * @param name  QString 技能名
     * @param user  PokemonBase* 使用者指针
     * @param dest  PokemonBase* 目标指针
     * @return result {AttackResult} 技能结果
     */
    static AttackResult 
        useSkillByName(
            const QString&  name,
            PokemonBase*    user = nullptr,
            PokemonBase*    dest = nullptr
        ) {
            if(!user || !dest) {
                qDebug() << "[PokemonSkill]: 目标或使用者为空";
                return AttackResult();
            }
            if(s_skillMap.count(name)) {
                return s_skillMap[name](user, dest);
            } else {
                qDebug() << "[PokemonSkill]: 不存在技能" << name;
                return AttackResult();
            }
        }
    // 高速移动
    SKILL_FUNC_DEF(Agility);
```  
### &emsp;&emsp;stage2
>对于用户注册请求的处理如下，先由serverBackend类分拣出对应事件码后调用注册处理函数并转发数据
  
部分分拣代码
```cpp
/**
 * @brief ServerBackend::slotGetMessage
 *        根据Global Message Type调用对应的处理槽函数
 * @param client    客户端TCP SOCKET
 * @param data      QByteArray封装的数据
 */
void ServerBackend::slotGetMessage(
    QTcpSocket*     client,
    QByteArray      data
) {
    // 拿到报文中的MessageType枚举
    const int type = *reinterpret_cast<int*>(data.data());
    
    switch (type) {
    case MessageType::UserSignUpRequest:
        CALL_SLOT(UserSignUp);
        break;
```
>具体细节见下文代码片以及注释
  
```cpp
// 处理用户注册请求
NET_SLOT(UserSignUp) {
    UserProtocol::UserSignUpRequestInfo reqInfo = {};
    reqInfo.ParseFromArray(data.data(), data.size());
    
    qDebug() << "[ServerBackend] UserSignUp"; 
#ifdef DEBUG_FLAG
    reqInfo.PrintDebugString();
#endif
    int     count = 0;
    QString userName = QString::fromStdString(reqInfo.username());
    
    // 检测用户名是否已经被占用
    StorageHelper::Instance().transaction(
        "SELECT count(*) FROM `user_list` WHERE USERNAME=?",
        [&count](QSqlQuery& query){
            count = query.value(0).toInt();
        },
        userName
    );
    
    // 构造发给客户端的response信息
    UserProtocol::UserSignUpResponseInfo resInfo = {};
    if(count > 0) {
#ifdef DEBUG_FLAG
        qDebug() << "[SERVER BACKEND-SIGN UP] user already exist";
#endif
        // 若count不为0，说明用户已注册，返回状态码USER_ALREADY_EXISTS
        resInfo.set_status(
            UserProtocol::UserSignUpResponseInfo_SignUpStatus_USER_ALREADY_EXISTS);
    } else {
        StorageHelper::Instance().transaction(
            "INSERT INTO user_list("
                "USERNAME, PASSWORD"
            ") VALUES(?, ?)", 
            StorageHelper::DEFAULT_FUNC,
            userName,
            QString::fromStdString(reqInfo.userpsw())
        );
        // 创建用户精灵表
        createUserTable(userName);
        
        QList<int> typeIdList = {};
        StorageHelper::Instance().transaction(
            "SELECT PKM_ID FROM `pokemon_info`", 
            [&typeIdList](QSqlQuery& query) {
                typeIdList.push_back(query.value(0).toInt());
            }
        );
        
        // 随机插入三个精灵
        for (auto i = 0; i < 3; i++) {
            int randIndex = 
                QRandomGenerator::global()->bounded(typeIdList.size());
            
            StorageHelper::Instance().transaction(
                "INSERT INTO `user_" +  userName + "`(\
                 PKM_TYPEID) VALUES(?)",
                 StorageHelper::DEFAULT_FUNC,
                 typeIdList[randIndex]
            );
        }
        
        resInfo.set_status(
            UserProtocol::UserSignUpResponseInfo_SignUpStatus_SUCCESS);
    }
    
    PROC_PROTODATA(UserSignUpResponse, resInfo);
}
```
>查看用户信息有专门提供的UserInfoRequest处理函数，在线用户则由ServerBackend类维护一个用户列表
  
```cpp
    QList<User>             /*用户列表*/
        m_userList = {};
    
    // 登录成功时，将用户加入用户列表
    UserProtocol::UserLoginResponseInfo resInfo = {};
    if(flag) {
        if(userPsw == QString::fromStdString(reqInfo.userpsw())) {
            resInfo.set_status(
                UserProtocol::UserLoginResponseInfo_LoginStatus_SUCCESS);
            for(int i = 0; i < m_userList.size(); i++) {
                if (userName == m_userList.at(i).get_name()) {
                    resInfo.set_status(
                        UserProtocol::UserLoginResponseInfo_LoginStatus_SERVER_REFUSED);
                    break;
                }
            }
            // TODO: 将 User 加入 UserList？ -- finished
            {
                User curUser(userName, client);
                m_userList.push_back(curUser);
            }
```

### &emsp;&emsp;stage3
>本系统的战斗采取每场战斗生成一个battlefiled类进行管理，ServerBackend维护一个BattleField列表，对战结束或有一方掉线&逃离时会向backend发出信号，调用对应的结果处理函数
```cpp
class BattleField: public QObject {
    Q_OBJECT
public RESOURCE:
    /**
     * @brief The BalanceType enum
     */
    enum BalanceType {
        A_TO_B = 0,
        B_TO_A
    };
    
    /**
     * @brief The BattleMode enum
     */
    enum BattleMode {
        EXP_BATTLE = 0,
        DUEL_BATTLE
    };
    
    /**
     * @brief The TurnInfo struct
     */
    struct TurnInfo {
        BalanceType type;           // A对B 或 B对A
        QString     skillName;      // 使用技能名
        int         selfDeltaHP;    // 自身hp变化量
        int         destDeltaHP;    // 对方hp变化量
        Buff        selfBuff;       // 自身获得的buff
        Buff        destBuff;       // 对方获得的buff
    };
    
public:
    /**
         * @brief BattleField
         * @param userA
         * @param userB
         * @param pkmA
         * @param pkmB
         * @param mode
         * @param parent
         */
        BattleField(
            User*           userA,
            User*           userB,
            PokemonBase*    pkmA,
            PokemonBase*    pkmB,
            BattleMode      mode,
            QObject*        parent = nullptr
        );
    virtual     /*析构对战类*/
        ~BattleField();
    
public FUNCTION:
    void                 /*设定回合行动*/
        setAction(int skillIndex, int userIndex);
    void                 /*进行一回合战斗*/
        turn(const QString& actionA, const QString& actionB);
	User*                /*获取USER A*/
		getUserA() const { return m_users[0]; }
	PokemonBase*        /*获取宝可梦A*/
		getPkmA() const  { return m_pkmList[0]; }
	User*               /*获取User B*/
		getUserB() const { return m_users[1]; }
	PokemonBase*        /*获取宝可梦B*/
		getPkmB() const  { return m_pkmList[1]; }
	BattleMode          /*获取对战类型： 升级 & 决斗*/
        getMode() const  { return m_mode; }
    
private FUNCTION:
    void    /*遍历BuffList 更新属性*/
        queryBuffList();
    bool    /*检测对战是否结束*/
        checkBattleIsFinished();
    void    /*进一步处理attack返回的AttackResult*/
        handleResult(AttackResult& result, BalanceType type);
    
signals:
    void    /*对战结束信号*/
        sigBattleFinished(User* winner);
	void    /*回合信息生成信号*/
		sigTurnInfoReady(TurnInfo info);
    
private RESOURCE:
    BattleMode                  /*对战模式*/
        m_mode;
    std::array<User*, 2>        /*用户指针*/
        m_users = {};
    std::array<PokemonBase*, 2> /*宝可梦指针*/
        m_pkmList = {};
    QList<Buff>                 /*Buff列表A & B*/
        m_buffListA = {},
        m_buffListB = {};
    std::array<QString, 2>      /*用户A & B设定的回合内行动*/
        m_actions = {};
};
```

处理对战结果，转移精灵的handler functor
```cpp
/**
 * @brief ServerBackend::slotGetBattleResult
 *        处理对战结果，析构并移除对应BattleField & 发送战利品信息 & 获得战利品 & 更新用户信息
 * @param winner {User*} 对战胜者
 */
void ServerBackend::slotGetBattleResult(User* winner) {
    BattleProtocol::BattleFinishInfo infoWinner = {};
    BattleProtocol::BattleFinishInfo infoLoser  = {};
    
    // 拿到发送方BattleField指针
    auto pBattleField = reinterpret_cast<BattleField*>(sender());
    // 获取用户指针
    auto pUserA = pBattleField->getUserA();
    auto pUserB = pBattleField->getUserB();
    
    // 生成胜利者与失败者
    User* loser = nullptr;
    if (winner == pUserA) {
        loser = pUserB;
    } else {
        loser = pUserA;
    }
    
    infoWinner.set_mode(BattleProtocol::BattleFinishInfo_FinishMode_NORMAL);
    infoWinner.set_result(BattleProtocol::BattleFinishInfo_BattleResult_WIN);
    
    infoLoser.set_mode(BattleProtocol::BattleFinishInfo_FinishMode_NORMAL);
    infoLoser.set_result(BattleProtocol::BattleFinishInfo_BattleResult_LOSE);
    
    // 如果是与server对战，则pUserB必定为nullptr
    // 更改用户状态
    if (loser != nullptr) {
        loser->set_status(User::UserStatus::IDLE);
        PROC_PROTODATA_WITH_DEST(
            BattleFinishInfo, infoLoser, loser->get_userSocket());
    }
    if (winner != nullptr) {
        winner->set_status(User::UserStatus::IDLE);
        // 不论对战模式，胜利方都将获取经验
        if (winner == pUserA) {
            pBattleField->getPkmA()->gainExperience(
                pBattleField->getPkmB()->get_level() * 1.5
            );
        } else {
            pBattleField->getPkmB()->gainExperience(
                pBattleField->getPkmA()->get_level() * 1.5
            );
        }
        PROC_PROTODATA_WITH_DEST(
            BattleFinishInfo, infoWinner, winner->get_userSocket());
    }
    
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // 如果为决斗战
    if (pBattleField->getMode() == BattleField::BattleMode::DUEL_BATTLE) {
        if (winner != nullptr) {
            winner->battleWon();
            
            if (loser != nullptr) {
                loser->battleLose();
                
                QString loserName = loser->get_name();
                auto    pkmIdList = loser->get_pokemonList();
                QList<int> selectIdList;
                
                // 生成要送出的精灵列表
                for (int i = 0; i < 3 && pkmIdList.length() > 0; i++) {
                    int randIndex = 
                        QRandomGenerator::global()->bounded(pkmIdList.length());
                    selectIdList.push_back(pkmIdList[randIndex]);
                    pkmIdList.removeAt(randIndex);
                }
                
                // 处理并发送战利品宝可梦信息
                procAndSendPkmData(
                    selectIdList,
                    loserName,
                    loser->get_userSocket(),
                    UserProtocol::PokemonDataRequestMode::TROPHIE
                );
                
// --------------------------------------------------------------------------------- //                
                
            } else {
                // 直接拿到对战精灵
                transferPokemon(
                    "_server",
                    winner->get_name(),
                    pBattleField->getPkmB()->get_id()
                );
            }
        } else {
            if (loser != nullptr) {
                loser->battleLose();

                QString loserName = loser->get_name();
                auto    pkmIdList = loser->get_pokemonList();
                QList<int> selectIdList;
                
                for (int i = 0; i < 3 && pkmIdList.length() > 0; i++) {
                    int randIndex = 
                        QRandomGenerator::global()->bounded(pkmIdList.length());
                    selectIdList.push_back(pkmIdList[randIndex]);
                    pkmIdList.removeAt(randIndex);
                }
                
                // 处理并发送战利品宝可梦信息
                procAndSendPkmData(
                    selectIdList,
                    loserName,
                    loser->get_userSocket(),
                    UserProtocol::PokemonDataRequestMode::TROPHIE
                );


            }
#ifdef DEBUG_FLAG
            else {
                qDebug() << "?";
            }
#endif
        }
    }
    
    // 断链信号并做清理
    disconnect(
        pBattleField,   &BattleField::sigTurnInfoReady,
        this,           &ServerBackend::slotGetTurnInfo
    );
    disconnect(
        pBattleField,   &BattleField::sigBattleFinished,
        this,           &ServerBackend::slotGetBattleResult
    );
    m_battleFieldList.removeOne(pBattleField);
    delete pBattleField;
}
```
>用户勋章
```cpp
// 变量定义
class User: public QObject {
    Q_OBJECT
public:
        User(                   /*User构造函数*/
			QString		username,
			QTcpSocket* socket
        );
		User(const User&);      /*User拷贝构造函数数*/
    virtual                     /*User析构函数*/
        ~User();                
public RESOURCE:
    /**
     * @brief The BadgeType enum
     */
	enum class BadgeType: int {
		BRONZE = 0,
		SILVER,
		GOLDEN
	};
    
    /**
     * @brief The UserStatus enum
     */
	enum class UserStatus: int {
		IDLE = 0,
		BATTLING,
        NOT_ONLINE,
		UNDEFINED,
	};

/**
 * @brief User::updateUserInfo
 *        更新用户信息
 */
void User::updateUserInfo() {
    // ----------UPDATE WIN RATE(?)-------------- //
    StorageHelper::Instance().transaction(
"SELECT TOTAL_BATTLE_TIME, WINNER_TIME \
    FROM `user_list` WHERE USERNAME=?\
",
    [this](QSqlQuery& query) {
        int totalWinnerTime = query.value(1).toInt(),
            totalBattleTime = query.value(0).toInt();
        this->m_winRate = 
            static_cast<double>(totalWinnerTime)/
            static_cast<double>(totalBattleTime);
    });
    
    StorageHelper::Instance().transaction(
        "SELECT * FROM user_" + m_name,
        [this](QSqlQuery& query) {
            this->m_pokemonList.push_back(query.value(0).toInt());
    });
    
    auto pokemonCnt = m_pokemonList.size();
    if(pokemonCnt >= 15) {
        m_countBadge = BadgeType::GOLDEN;
    } else if(pokemonCnt >= 7) {
        m_countBadge = BadgeType::SILVER;
    } else {
        m_countBadge = BadgeType::BRONZE;
    }
    
    int highLevelCnt = 0;
    StorageHelper::Instance().transaction(
        "SELECT count(*) FROM `user_" + m_name + "` WHERE PKM_LEVEL=15",
        [&highLevelCnt](QSqlQuery query) {
            highLevelCnt = query.value(0).toInt();
    });
    if(highLevelCnt >= 10) {
        m_qualityBadge = BadgeType::GOLDEN;
    } else if(highLevelCnt >= 5) {
        m_qualityBadge = BadgeType::SILVER;
    } else {
        m_qualityBadge = BadgeType::BRONZE;
    }
    qDebug() << "update user " + m_name + " info finished";
    
}
```
>不确定性
```cpp
// BF内server随机出招
for (int i = 0; i < m_battleFieldList.size(); i++) {
        auto battle = m_battleFieldList[i];
        if (isUserA) {
            if (battle->getUserA()->get_name() == userName) {
                battle->setAction(skillIndex, 0);
                if (battle->getUserB() == nullptr) {
                    battle->setAction(
                        QRandomGenerator::global()->bounded(4), 1);
                }
            }
        } else {
            if (battle->getUserB()->get_name() == userName) {
                battle->setAction(skillIndex, 1);
            }
        }
    }

// attack的不确定性
AttackResult HighAtkPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    AttackResult&& result = PokemonSkill::useSkillByName(skillName, this, &target);
    
    JUDGE_OZ(20) {
        if(result.destDeltaHp)
            result.destDeltaHp *= 1.2;
    }
    
    return result;
}

// 技能的不确定性
REGISTER_SKILL(AppleAcid) {
    ATK_DEBUG(苹果酸);
    AttackResult result;
    JUDGE_OZ(80) {
        result.destBuf.buffId  = PokemonBase::BuffType::DEF_DOWN_M;
        result.destBuf.turnCnt = 2;
    }
    
    result.destDeltaHp = DAMAGE(80);
    if JUDGE_ATTR(FIRE) {
        result.destDeltaHp *= 1.2;
    } else if JUDGE_ATTR(GRASS) {
        result.destDeltaHp *= 0.8;
    }
    
    return result;
}
```

## 5. 实现效果预览
### SERVER
#### WIN10_DESKTOP
&emsp;&emsp;**关于LOG没做时间戳和模块细分不够完备这两个问题，本人深表忏悔**  

启动
![启动界面](https://img-blog.csdnimg.cn/20200811015804539.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NTg3NDYz,size_16,color_FFFFFF,t_70)
对战LOG
![](https://img-blog.csdnimg.cn/20200811015830147.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NTg3NDYz,size_16,color_FFFFFF,t_70)
对战结束
![](https://img-blog.csdnimg.cn/20200811015842240.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NTg3NDYz,size_16,color_FFFFFF,t_70)
### CLIENT
#### ANDROID OS_BUILD_VERSION_10
&emsp;&emsp;**以下图片内容包含 初始界面 宝可梦详情界面 用户信息界面 对战界面 弹窗样式等**  
![](https://img-blog.csdnimg.cn/20200811021019843.png)![](https://img-blog.csdnimg.cn/20200811021041131.png)![](https://img-blog.csdnimg.cn/20200811021054227.png)![](https://img-blog.csdnimg.cn/20200811021107561.png)![](https://img-blog.csdnimg.cn/20200811021117194.png)![](https://img-blog.csdnimg.cn/20200811021128759.png)![](https://img-blog.csdnimg.cn/20200811021143340.png)![](https://img-blog.csdnimg.cn/20200811021155340.png)![](https://img-blog.csdnimg.cn/20200811021206831.png)![](https://img-blog.csdnimg.cn/20200811021220189.png)
#### WIN10_DESKTOP
&emsp;&emsp;**以下图片内容包含 登录&注册界面 用户列表 对战邀请弹窗样式等**  
![](https://img-blog.csdnimg.cn/20200811021324563.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NTg3NDYz,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20200811021337433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NTg3NDYz,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20200811021353693.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NTg3NDYz,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20200811021403180.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NTg3NDYz,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20200811021421215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NTg3NDYz,size_16,color_FFFFFF,t_70)
>仍待补充 (如选择战利品界面等) ，上图中存在过渡界面 (如等待建立对战) 在各处请求网络时都会存在，但由于测试在内网环境下进行，故暂时未保存各处的对应截图