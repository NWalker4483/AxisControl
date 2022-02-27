#include <merlin_control/merlin_control.h>

void MerlinArm()
{
  std::cout << "Starting Merlin Arm setup process\n";
  size_t portCount = 0;
  std::vector<std::string> comHubPorts;

  try
  {
    SysManager::FindComHubPorts(comHubPorts); // Find Usb Ports with Hub
    printf("Found %d SC Hubs\n", comHubPorts.size());
    for (portCount = 0; portCount < comHubPorts.size() && portCount < NET_CONTROLLER_MAX; portCount++)
    {
      myMgr.ComHubPort(portCount, comHubPorts[portCount].c_str()); // define the first SC Hub port (port 0) to be associated
                                                                   // with COM portnum (as seen in device manager)
    }
    if (portCount < 0)
    {
      printf("Unable to locate SC hub port\n");
      msgUser("Press any key to continue."); // pause so the user can see the error message; waits for user to press a key
      // TODO: Raise Error
      return -1; // This terminates the main program
    }

    myMgr.PortsOpen(portCount); // Open the port
    for (size_t i = 0; i < portCount; i++)
    {
      IPort &myPort = myMgr.Ports(i);
      printf(" Port[%d]: state=%d, nodes=%d\n",
             myPort.NetNumber(), myPort.OpenState(), myPort.NodeCount());
      // Once the code gets past this point, it can be assumed that the Port has been opened without issue
      // Now we can get a reference to our port object which we will use to access the node objects
      for (size_t iNode = 0; iNode < myPort.NodeCount(); iNode++)
      {
        // Create a shortcut reference for a node
        INode &theNode = myPort.Nodes(iNode);
        theNode.EnableReq(false); // Ensure Node is disabled before loading config file

        // The following statements will attempt to enable the node.  First,
        //  any shutdowns or NodeStops are cleared, finally the node is enabled
        theNode.Status.AlertsClear();   // Clear Alerts on node
        theNode.Motion.NodeStopClear(); // Clear Nodestops on Node
        theNode.EnableReq(true);        // Enable node
        double timeout = myMgr.TimeStampMsec() + TIME_TILL_TIMEOUT; // define a timeout in case the node is unable to enable
                                                                    // This will loop checking on the Real time values of the node's Ready status
        while (!theNode.Motion.IsReady())
        {
          if (myMgr.TimeStampMsec() > timeout)
          {
            printf("Error: Timed out waiting for Node %d to enable\n", iNode);
            msgUser("Press any key to continue."); // pause so the user can see the error message; waits for user to press a key
            return -2;
          }
        }
      }
    }
  }
  catch (mnErr &theErr)
  {
    printf("Failed to disable Nodes n\n");
    // This statement will print the address of the error, the error code (defined by the mnErr class),
    // as well as the corresponding error message.
    printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n", theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
    msgUser("Press any key to continue."); // pause so the user can see the error message; waits for user to press a key
    return 0;                              // This terminates the main program
  }
}

~MerlinArm()
{
  for (size_t i = 0; i < portCount; i++)
  {
    IPort &myPort = myMgr.Ports(i);
    printf("Disabling nodes, and closing port\n");
    // Disable Nodes

    for (size_t iNode = 0; iNode < myPort.NodeCount(); iNode++)
    {
      // Create a shortcut reference for a node
      myPort.Nodes(iNode).EnableReq(false);
    }
    // Close down the ports
    myMgr.PortsClose();
  }
}

void MerlinArm::setMotorReductions(double reductions[6][6])
{
  for (int row = 0; row < 6; row++)
  {
    for (int col = 0; col < 6; col++)
    {
      motor_reductions(row, col) = reductions[row][col];
    }
  }
  // Pre Compute Useful Variables
  degrees_per_step = motor_reductions * (360.L / 800.L);
  Invert(degrees_per_step, degrees_per_step_inv);
}

void MerlinArm::setMotorSpeeds() override
{
  // Define
  for (size_t iNode = 0; iNode < myPort.NodeCount(); iNode++)
  {
    // Create a shortcut reference for a node
    INode &theNode = myPort.Nodes(iNode);
    theNode.Motion.MoveWentDone();                 // Clear the rising edge Move done register
    theNode.AccUnit(INode::RPM_PER_SEC);           // Set the units for Acceleration to RPM/SEC
    theNode.VelUnit(INode::RPM);                   // Set the units for Velocity to RPM
    theNode.Motion.AccLimit = ACC_LIM_RPM_PER_SEC; // Set Acceleration Limit (RPM/Sec)
    theNode.Motion.VelLimit = VEL_LIM_RPM;         // Set Velocity Limit (RPM)
    printf("Moving Node \t%zi \n", iNode);
    theNode.Motion.MovePosnStart(MOVE_DISTANCE_CNTS); // Execute 10000 encoder count move
    printf("%f estiomated time.\n", theNode.Motion.MovePosnDurationMsec(MOVE_DISTANCE_CNTS));
    double timeout = myMgr.TimeStampMsec() + theNode.Motion.MovePosnDurationMsec(MOVE_DISTANCE_CNTS) + 100; // define a timeout in case the node is unable to enable
    while (!theNode.Motion.MoveIsDone())
    {
      if (myMgr.TimeStampMsec() > timeout)
      {
        printf("Error: Timed out waiting for move to complete\n");
        msgUser("Press any key to continue."); // pause so the user can see the error message; waits for user to press a key
        return -2;
      }
    }
    printf("Node \t%zi Move Done\n", iNode);
  } // for each node
}

void MerlinArm::computeJointPositions() override
{
 
  Matrix<double, 6, 1> steps_taken;
  for (int i = 0; i < 6; i++)
  {
    myNode.Motion.PosnMeasured;
    steps_taken(i) = // stepper[i]->currentPosition();
  }
  
   Matrix<double, 6, 1> current_poses = degrees_per_step * steps_taken;
}

void MerlinArm::updateMotorSpeeds() override
{
  Matrix<double, 6, 1> speeds_holder;
  Matrix<double, 6, 1> steps_per_second;

  for (int i = 0; i < 6; i++)
    speeds_holder(i) = _speeds[i];

  steps_per_second = degrees_per_step_inv * speeds_holder;

  for (int i = 0; i < 6; i++)
  {
    // Set Node RPM
    stepper[i]->setSpeed(steps_per_second(i));
  }
}

unsigned int MerlinArm::getTime() override {
  return myMgr.TimeStampMsec();
  }

pass