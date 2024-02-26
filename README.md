基于Oimo框架制作的游戏服务器.

Oimo框架地址：[https://github.com/you-not-fish/oimo](https://github.com/you-not-fish/oimo)

游戏客户端来自罗培羽老师的《Unity3D网络游戏实战第2版》中的最终案例.

客户端地址：[https://luopeiyu.github.io/unity_net_book/](https://luopeiyu.github.io/unity_net_book/)

本demo包括登陆模块，房间模块，战斗模块.

服务器完全基于actor模型实现，主要包括以下几个独立service（目前还没有更新数据库模块，注册没有效果，密码为123便可登陆成功）：

1. gateway服务：网关服务，主要负责网络连接，序列化和反序列化，消息转发；
2. login服务：登陆服务，处理登陆消息和注册消息；
3. agent服务：客户端代理模块，接受并处理网关服务转发的客户端消息；
4. agentmgr服务：管理agent的创建、销毁；
5. room服务：房间服务，当玩家创建一个房间时，服务器就会启动一个room服务处理房间内的消息；
6. roommggr服务：管理房间内的创建和销毁.
