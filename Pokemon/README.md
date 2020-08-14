# 基于QtTcpSocket protobuf c++17 实现的C/S模式宝可梦在线对战游戏系统
## 目录
1. 开发环境及配置说明
2. 系统设计介绍
3. 关于【运行】以及【自定义内容】（自定义宝可梦属性 & 自定义技能）的说明
4. 实现效果预览

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

## 4. 实现效果预览
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