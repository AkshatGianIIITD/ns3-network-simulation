#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include <map>
#include <string>
#include <vector>
#include "ns3/error-model.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("NetworkTopologySimulation");

int main(int argc, char *argv[]) {

    //Creating mapping for traffic from source to destination nodes according to the traffic table created by us.
    std::map<std::string, std::map<std::string, int>> trafficMatrix;

    // Fill the traffic matrix
    trafficMatrix["A"]["A"] = 0;
    trafficMatrix["A"]["B"] = 124;
    trafficMatrix["A"]["C"] = 95;
    trafficMatrix["A"]["D"] = 56;
    trafficMatrix["A"]["E"] = 57;

    trafficMatrix["B"]["A"] = 83;
    trafficMatrix["B"]["B"] = 0;
    trafficMatrix["B"]["C"] = 30;
    trafficMatrix["B"]["D"] = 17;
    trafficMatrix["B"]["E"] = 55;

    trafficMatrix["C"]["A"] = 78;
    trafficMatrix["C"]["B"] = 144;
    trafficMatrix["C"]["C"] = 0;
    trafficMatrix["C"]["D"] = 84;
    trafficMatrix["C"]["E"] = 60;

    trafficMatrix["D"]["A"] = 59;
    trafficMatrix["D"]["B"] = 33;
    trafficMatrix["D"]["C"] = 46;
    trafficMatrix["D"]["D"] = 0;
    trafficMatrix["D"]["E"] = 133;

    trafficMatrix["E"]["A"] = 23;
    trafficMatrix["E"]["B"] = 14;
    trafficMatrix["E"]["C"] = 148;
    trafficMatrix["E"]["D"] = 44;
    trafficMatrix["E"]["E"] = 0;

    

    // Enable logging
    LogComponentEnable("NetworkTopologySimulation", LOG_LEVEL_INFO);

    // Create nodes
    NodeContainer routers;
    routers.Create(4); // R0, R1, R2, R3

    NodeContainer workstations;
    workstations.Create(5); // A, B, C, D, E

    // Create node containers for point-to-point connections
    NodeContainer aToR1 = NodeContainer(workstations.Get(0), routers.Get(1)); // A -> R1
    NodeContainer r1ToA = NodeContainer(routers.Get(1),workstations.Get(0));
    NodeContainer r1ToR0 = NodeContainer(routers.Get(1), routers.Get(0)); // R1 -> R0
    NodeContainer r2ToR0 = NodeContainer(routers.Get(2), routers.Get(0));      // R2 -> R0
    NodeContainer r3ToR0 = NodeContainer(routers.Get(3), routers.Get(0));      // R3 -> R0
    NodeContainer bToR2 = NodeContainer(workstations.Get(1), routers.Get(2));  // B -> R2
    NodeContainer cToR2 = NodeContainer(workstations.Get(2), routers.Get(2));  // C -> R2
    NodeContainer dToR3 = NodeContainer(workstations.Get(3), routers.Get(3));  // D -> R3
    NodeContainer eToR3 = NodeContainer(workstations.Get(4), routers.Get(3));  // E -> R3

    // Install Internet stack
    InternetStackHelper internet;
    internet.Install(routers);
    internet.Install(workstations);

    std::string workstationsArr[5] = {"A", "B", "C", "D", "E"};


    // Define point-to-point connections with data rates and delays
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("1ms"));

    p2p.SetQueue("ns3::DropTailQueue<Packet>","MaxSize", QueueSizeValue(QueueSize("50p")));    
    
    std::vector<NetDeviceContainer> devices;

    // Create point-to-point links and install them
    NetDeviceContainer devicesAR1 = p2p.Install(aToR1);
    devices.push_back(devicesAR1);

 
    p2p.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("1ms"));
    NetDeviceContainer devicesR1A = p2p.Install(r1ToA);


    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("1ms"));
    NetDeviceContainer devicesR1R0 = p2p.Install(r1ToR0);
    devices.push_back(devicesR1R0);


    NetDeviceContainer devicesR2R0 = p2p.Install(r2ToR0);
    devices.push_back(devicesR2R0);
   
    NetDeviceContainer devicesR3R0 = p2p.Install(r3ToR0);
    devices.push_back(devicesR3R0);

    p2p.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("1ms"));
    NetDeviceContainer devicesBR2 = p2p.Install(bToR2);
    devices.push_back(devicesBR2);

    NetDeviceContainer devicesCR2 = p2p.Install(cToR2);
    devices.push_back(devicesCR2);

    NetDeviceContainer devicesDR3 = p2p.Install(dToR3);
    devices.push_back(devicesDR3);

    NetDeviceContainer devicesER3 = p2p.Install(eToR3);
    devices.push_back(devicesER3);   
    

    for(uint32_t i=0; i<devices.size();i++){
    Ptr<NetDevice> device1 = devices[i].Get(0);
    Ptr<NetDevice> device2 = devices[i].Get(1);
    Ptr<PointToPointNetDevice> p2pDevice1 = DynamicCast<PointToPointNetDevice>(device1);
    p2pDevice1->SetMtu(2048);
    Ptr<PointToPointNetDevice> p2pDevice2 = DynamicCast<PointToPointNetDevice>(device2);
    p2pDevice2->SetMtu(2048);
    

}   
   // NetFlowHelper netFlowHelper;
    
    // Install the NetFlow helper on all devices (or specific ones)
    //netFlowHelper.InstallAll();
    //
    //Assigning Error Models
    Ptr<RateErrorModel> errorModel1 = CreateObject<RateErrorModel>();
    errorModel1->SetAttribute("ErrorRate", DoubleValue(0.0075)); 
    // Ptr<RateErrorModel> errorModel2 = CreateObject<RateErrorModel>();
    // errorModel2->SetAttribute("ErrorRate", DoubleValue(0.008)); 

    devicesAR1.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesAR1.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesR1A.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesR1A.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesR1R0.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesR1R0.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesR2R0.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesR2R0.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesR3R0.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesR3R0.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesBR2.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesBR2.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesCR2.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesCR2.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesDR3.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesDR3.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesER3.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    devicesER3.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(errorModel1));
    // Assign IP addresses
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesAR1 = ipv4.Assign(devicesAR1);

    ipv4.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesR2R0 = ipv4.Assign(devicesR2R0);

    ipv4.SetBase("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesR3R0 = ipv4.Assign(devicesR3R0);

    ipv4.SetBase("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesBR2 = ipv4.Assign(devicesBR2);

    ipv4.SetBase("10.1.5.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesCR2 = ipv4.Assign(devicesCR2);

    ipv4.SetBase("10.1.6.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesDR3 = ipv4.Assign(devicesDR3);

    ipv4.SetBase("10.1.7.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesER3 = ipv4.Assign(devicesER3);

    //Assigning IP for R1toR0
    ipv4.SetBase("10.1.8.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesR1R0 = ipv4.Assign(devicesR1R0);

    //Asssignint IP for R1 to A
    ipv4.SetBase("10.1.9.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesR1A = ipv4.Assign(devicesR1A);

    // Enable routing
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Install traffic generators (UDP Echo Server/Client)
    uint16_t port = 8080;

    
    Ipv4Address workstationsAddressArr[5] = {interfacesAR1.GetAddress(0),interfacesBR2.GetAddress(0),interfacesCR2.GetAddress(0),interfacesDR3.GetAddress(0),interfacesER3.GetAddress(0)};
    for(int i=0;i<5;i++){   //Client Number 
        for(int j=0;j<5;j++){  //Server Number
            UdpEchoServerHelper echoServer(++port);

            ApplicationContainer serverApps = echoServer.Install(workstations.Get(j)); // Server on B
            serverApps.Start(Seconds(1.0));
            serverApps.Stop(Seconds(60.0));

            UdpEchoClientHelper echoClient(workstationsAddressArr[j], port); //server address (destination)
            echoClient.SetAttribute("MaxPackets", UintegerValue(trafficMatrix[workstationsArr[i]][workstationsArr[j]]));
            echoClient.SetAttribute("Interval", TimeValue(Seconds(0.01)));
            echoClient.SetAttribute("PacketSize", UintegerValue(2048));

            ApplicationContainer clientApps = echoClient.Install(workstations.Get(i)); // Client on A
            clientApps.Start(Seconds(2.0));
            clientApps.Stop(Seconds(61.0));
        }
    }

    // Enable NetAnim tracing
    AnimationInterface anim("network_topology_with_positions2.xml");

    anim.SetConstantPosition(routers.Get(0), 45.0,45.0); //R1
    anim.SetConstantPosition(routers.Get(1), 45.0,30.0); //R2
    anim.SetConstantPosition(routers.Get(2), 30.0,60.0); //R3
    anim.SetConstantPosition(routers.Get(3), 60.0,60.0); //R0

    anim.SetConstantPosition(workstations.Get(0), 45.0,20.0); //A
    anim.SetConstantPosition(workstations.Get(1), 15.0,55.0); //B
    anim.SetConstantPosition(workstations.Get(2), 15.0,65.0); //C
    anim.SetConstantPosition(workstations.Get(3), 75.0,65.0); //D
    anim.SetConstantPosition(workstations.Get(4), 75.0,55.0); //E
    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll(ascii.CreateFileStream("first.tr"));
    p2p.EnablePcapAll("topology");
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}