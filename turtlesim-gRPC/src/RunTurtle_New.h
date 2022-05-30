void RunTurtle() {
        
        auto node = rclcpp::Node::make_shared("turtle_controller");
        auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",1);

        rclcpp::WallRate loop(10000);
        while (rclcpp::ok()) {
                auto msg = geometry_msgs::msg::Twist();
                msg.linear.x = 2.0;
                msg.angular.z = 2.0;
                publisher->publish(msg);
                loop.sleep();
        }

}
