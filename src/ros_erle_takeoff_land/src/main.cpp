#include <cstdlib>
#include <ros/ros.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/SetMode.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int rate = 10;
    ros::init(argc, argv, "mavros_takeoff");
    ros::NodeHandle n;

    ros::Rate r(rate);

    char input;
    while(true)
    {
        printf("(S)etmode to GUIDED, (A)rm the throttle, (T)akeoff, (L)and, or (E)xit:\n");
        scanf(" %c", &input);
        if(input == 'S' || input == 's')
        {
            // Setting mode to GUIDED: CMDLINE ->> mode guided
            ros::ServiceClient cl = n.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
            mavros_msgs::SetMode srv_setMode;
            srv_setMode.request.base_mode = 0;
            srv_setMode.request.custom_mode = "GUIDED";
            if(cl.call(srv_setMode)){
                ROS_INFO("setmode send ok");
            }else{
                ROS_ERROR("Failed SetMode");
                return -1;
            }
        }
        else if(input == 'A' || input == 'a')
        {
            // Arming the throttle: CMDLINE ->> arm throttle
            ros::ServiceClient arming_cl = n.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
            mavros_msgs::CommandBool srv;
            srv.request.value = true;
            if(arming_cl.call(srv)){
                ROS_INFO("ARM send ok");
            }else{
                ROS_ERROR("Failed arming or disarming");
            }
        }
        else if(input == 'T' || input == 't')
        {
            // Taking off: CMDLINE ->> takeoff #(height in meters)
            ros::ServiceClient takeoff_cl = n.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/takeoff");
            mavros_msgs::CommandTOL srv_takeoff;
            srv_takeoff.request.altitude = 10;
            srv_takeoff.request.latitude = 0;
            srv_takeoff.request.longitude = 0;
            srv_takeoff.request.min_pitch = 0;
            srv_takeoff.request.yaw = 0;
            if(takeoff_cl.call(srv_takeoff)){
                ROS_INFO("srv_takeoff send ok");
            }else{
                ROS_ERROR("Failed Takeoff");
            }
            sleep(10);
        }
        else if(input == 'L' || input == 'l')
        {
            // Land: CMDLINE ->> mode land
            ros::ServiceClient land_cl = n.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/land");
            mavros_msgs::CommandTOL srv_land;
            srv_land.request.altitude = 10;
            srv_land.request.latitude = 0;
            srv_land.request.longitude = 0;
            srv_land.request.min_pitch = 0;
            srv_land.request.yaw = 0;
            if(land_cl.call(srv_land)){
                ROS_INFO("srv_land send ok");
            }else{
                ROS_ERROR("Failed Land");
            }
            sleep(10);
        }
        else if(input == 'E' || input == 'e')
        {
            exit(0);
        }
        else
        {
            printf("Invalid command!\n");;
        }
    }    

    while (n.ok())
    {
      ros::spinOnce();
      r.sleep();
    }

    return 0;
}