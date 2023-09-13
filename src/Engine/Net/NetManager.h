#pragma once
#include <memory>

#include "../../Network/Client.h"
#include "../../Network/Server.h"
class NetGameObject;

/**
 * \brief 网络功能
 *
 *  joinGame/ hostGame会阻塞线程直到连接服务器
 *  example
    * 	NetManager::GetInstance()->HostGame();
        ThreadPool::AddTask([&]()
        {
            this_thread::sleep_for(std::chrono::seconds(3));
            GameObject.AddToNet(NetRole::Owner,false);//玩家小球为owner 第二个参数是否为客户端
        });
 */
class NetManager
{
public:
    static NetManager* GetInstance();
    void HostGame();
    void JoinGame();
    void SendMsg();
    void Quit();
    bool HasServer();
    bool IsConnect();
    bool IsServer = false;

    std::map<uint16_t,std::string> Actor_type_map;    //no use
    UpdateActorInfo* AddNetGameObject(NetGameObject * game_object,uint16_t ClassTypeID);
private:

    bool stop = false;
    void StartSendMsg();
    std::thread send_thread;
    int64_t server_net_id = -1;
    int64_t client_net_id = -1;
    std::shared_ptr<Client> client_;
    std::shared_ptr<Server> server_;
    NetManager(){};
    static NetManager* _instance;
};
