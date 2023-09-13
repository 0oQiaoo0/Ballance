#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Connection.h"
#include <string.h>
#include <event2/buffer.h>
#include "Connection.h"
#include <iostream>
#include <string>

#include "Router.h"
#define addr_any_t "0.0.0.0"
#define IP_ADDR "127.0.0.1"
#define PORT 8811



Router::Router()
{
	//connection = new Connection();
}

Router::~Router()
{
	quit();
}

void Router::handleRead(std::string& addr, unsigned short port, std::string& data){
	std::cout<<data<<std::endl;
}
void Router::handleHead(RouterHead head,std::string& addr, unsigned short port, std::string& buffer)
{
	//client request
	switch (head.head_type)
	{
	case HEAD_SERVER:
		{
			HandleHeadServer(head,addr,port,buffer);
			break;
		}
	case HEAD_CLIENT:
		{
			HandleHeadClient(head,addr,port,buffer);
			break;
		}
	case HEAD_NONE:
		{
			HandleHeadNone(head,addr,port,buffer);
			break;
		}
		default:
		{
			break;
		}
	}
}

void Router::HandleHeadServer(RouterHead& head, std::string& addr, unsigned short port, std::string& buffer)
{
	switch (head.action)
	{
	case ACTION_REQUEST:
		{
			std::cout<<"server request"<<std::endl;
			g_net_id++;
			head.net_id =g_net_id;
			head.action = ACTION_RESPONSE;
			char * a =const_cast<char *> (buffer.data());
			memcpy(a,&head,sizeof(head));
			connection.send(addr,port,buffer);
			break;
		}
	case ACTION_ACK:
		{
			std::cout<<addr<<" "<<port<<" is server "<<std::endl;
			server_id = head.net_id;
			server_life_time = TIMEOUT_SECOND;
			endPoint_list[server_id] = std::make_shared<EndPoint>(server_id,addr,port);
			break;
		}
	case ACTION_OTHER:
		{
			if(head.net_id!=server_id)
			{
				std::cout<<head.net_id<<" is not the server"<<std::endl;
			}
			server_life_time = TIMEOUT_SECOND;
			for(auto id:client_id_list)
			{
				std::string& _addr =endPoint_list[id]->addr;
				unsigned short _port = endPoint_list[id]->port;
				connection.send(_addr,_port,buffer);
#if MY_DEBUG
				std::cout<<"send client msg to port "<<_port<<std::endl;
#endif
				
			}
		
		}
	}
}

void Router::HandleHeadClient(RouterHead& head, std::string& addr, unsigned short port, std::string& buffer)
{
	switch (head.action)
	{
	case ACTION_REQUEST:
		{
			std::cout<<"client request"<<std::endl;
			g_net_id++;
			head.net_id =g_net_id;
			head.action = ACTION_RESPONSE;
			char * a =const_cast<char *> (buffer.data());
			memcpy(a,&head,sizeof(head));
			connection.send(addr,port,buffer);
			
			break;
		}
	case ACTION_ACK:
		{
			std::cout<<addr<<" "<<port<<" is client "<<std::endl;
			client_id_list.push_back(head.net_id);
			client_life_time[head.net_id] = TIMEOUT_SECOND;
			endPoint_list[head.net_id] = std::make_shared<EndPoint>(head.net_id,addr,port);
			break;
		}
	case ACTION_OTHER:
		{
			if(server_id==-1||endPoint_list.count(server_id)<=0){
				std::cout<<"no server id or no client not send msg "<<std::endl;
				break;
			}
			if(--check_time<0&&client_life_time.count(head.net_id)>0)
			{
				check_time = TIMEOUT_CHECK_FREQUENT*max(client_life_time.size(),1);
				client_life_time[head.net_id] = TIMEOUT_SECOND;
			}
			std::string& _addr =endPoint_list[server_id]->addr;
			unsigned short _port = endPoint_list[server_id]->port;
			connection.send(_addr,_port,buffer);
#if MY_DEBUG
			std::cout<<"send client msg to server"<<std::endl;
#endif
			
		}
	}
}

void Router::HandleHeadNone(RouterHead& head, std::string& addr, unsigned short port, std::string& buffer)
{
	std::string rec = "invalid buffer:"+buffer;
	connection.send(addr,port,rec);
}


void Router::run()
{
	connection.set_recv_handler([&](std::string& addr, unsigned short port, std::string& buffer)
	{
#if MY_DEBUG
		std::cout<<"recv from addr: "<<addr<<"port:"<<port<<"data:"<<buffer<<std::endl;
#endif
		
		RouterHead head;
		memcpy(&head,buffer.data(),sizeof(head));
		handleHead(head,addr,port,buffer);
	});
	auto thread_ =  std::thread([&]()
	{
		std::cout<<"router local port: "<<PORT<<std::endl;
		connection.connect(PORT, addr_any_t);
	});

	auto heart_beat =  std::thread([&]()
	{
		while(!stop)
		{
			std::time_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout<<"router heart beat ... "<<std::to_string(timestamp)<<std::endl;
			if(server_id!=-1&&server_life_time--<=0)
			{
				std::cout<<"server : "<<server_id<<" disconnect"<<std::endl;
				server_id=-1;
				if(endPoint_list.count(server_id)>0)
				{
					endPoint_list.erase(server_id);
				}
			}
			std::vector<int64_t> client_to_remove;
			for(auto& client:client_life_time)
			{
				if(client.second--<0)
				{
					client_to_remove.push_back(client.first);
				}
			}
			for(auto client_id:client_to_remove)
			{
				for(auto it = client_id_list.begin();it!=client_id_list.end();)
				{
					if(  *it==client_id )
					{
						endPoint_list.erase(client_id);
						std::cout<<"client : "<<client_id<<" disconnect"<<std::endl;
						it =  client_id_list.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
		}
	});
	thread_.join();
	heart_beat.join();
	
}

void Router::quit()
{
	connection.close();
	stop =true;
}
