# 基于QtTcpSocket protobuf c++17 实现的C/S模式宝可梦在线对战游戏系统
## 目录
1. 实现效果预览
2. 开发环境及配置说明
## 1. 实现效果预览
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