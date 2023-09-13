#include "Server.h"
int main()
{
    Server s;

    UpdateActorInfo* update_actor_info_ =nullptr;
    s.OnNewNetActorJoin([&](UpdateActorInfo* update_actor_info)
      {
          std::cout<<"set update_actor_info_  "<<std::endl;
          update_actor_info_ = update_actor_info;
      });
    s.Start();
    std::thread t;

  
    while(true)
    {
        if(update_actor_info_!=nullptr)
        {
            float vec3[3];
            memcpy(&vec3,update_actor_info_->actor_info.transfer_data,sizeof(vec3));
            std::cout<<"x: "<<vec3[0]<<"y: "<<vec3[1]<<"z: "<<vec3[2]<<std::endl;
        }
        else
        {
            std::cout<<"update_actor_info_ empty "<<std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
}
