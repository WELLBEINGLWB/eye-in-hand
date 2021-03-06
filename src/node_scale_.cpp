// using namespace std;
#include <ptam_scale.h>
#include <Scale_estimation.hpp>


int main(int argc, char **argv)
{
	ros::init(argc, argv, "PtamScale_ROBOT");
	PtamScale node;
	
	ROS_INFO("[PtamScale_ROBOT] Node is ready");

	double spin_rate = 50;
	ros::param::get("~spin_rate",spin_rate);
	
	ROS_DEBUG( "Spin Rate %lf", spin_rate);

	cv::namedWindow("PtamScale_ROBOT");

	ros::Rate rate(spin_rate); 
		
	while ( ros::ok() )
    {
        rate.sleep(); 
        ros::spinOnce();            
    }

	return 0;
}

PtamScale::PtamScale()
{
	myfile4.open("/home/daniela/code/src/eye_in_hand/ptam_pose31.txt");
	ptam_sub = nh.subscribe("/vslam/pose",1, &PtamScale::SOtreCamera, this);  //word in camera framebu
	movewebcamrobot = nh.subscribe("/moverobot",1, &PtamScale::RobotMove,this); // robot in cam frame
	stop_sub = nh.subscribe("/stopandgo",1,&PtamScale::StopCallback,this);	//to stop the pc2 callback
	pub_scala = nh.advertise<std_msgs::Float32>("/scala_", 1);
	pub_scala_naif = nh.advertise<std_msgs::Float32>("/scala_naif_method", 1);
	So3_prev_ptam = KDL::Frame::Identity();
}

void PtamScale::StopCallback(const std_msgs::Bool::ConstPtr& msg)
{
	ROS_INFO_STREAM("arrivato msg stopandgo");
	stop_flag = msg->data;
}

void PtamScale::RobotMove(const geometry_msgs::Pose msg)
{
	tf::poseMsgToKDL(msg, Move_robot);
	So3_prev_ptam = frame_so3_ptam;
}


void PtamScale::SOtreCamera(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr msg)
{	 
	tf::poseMsgToKDL ((msg->pose).pose, frame_so3_ptam);
	frame_w_c = frame_so3_ptam.Inverse();  

	if(stop_flag == false)
	{ 
		KDL::Frame Frame_c2_c1;
		Frame_c2_c1 = So3_prev_ptam*frame_w_c;
		ROS_INFO_STREAM("Frame_c2_c1: "<< Frame_c2_c1.p.z());

		if(myfile4.is_open())
		{
			myfile4 << Move_robot.p.z()<<'\t';
			myfile4 << Frame_c2_c1.p.z() << '\n';
		}
		
		else
			ROS_INFO_STREAM("no myfile4");

		Robot.push_back(Move_robot.p.z());
		Ptam.push_back(Frame_c2_c1.p.z());
		double scala = Scale(Robot,Ptam);
		ROS_INFO_STREAM("scala: " << scala);
		Vect_scala.push_back(scala);
		
		if( Vect_scala.size() >= 1000 )
		{	
			if( (Vect_scala.back() - Vect_scala[Vect_scala.size() - 2]) <= 0.0001)
			{
				std_msgs::Float32 scala_;
				scala_.data = Vect_scala.back();
				pub_scala.publish(scala_);
				stop_flag = true;
				double scala_mio = ScalaReturn(Frame_c2_c1.p.z(), So3_prev_ptam.p.z(), Move_robot.p.z());
				ROS_INFO_STREAM("scala_mio: " << scala_mio);
				std_msgs::Float32 scala_mio_msg;
				scala_mio_msg.data = scala_mio;
				pub_scala_naif.publish(scala_mio_msg);
			}
		}
	}
}


