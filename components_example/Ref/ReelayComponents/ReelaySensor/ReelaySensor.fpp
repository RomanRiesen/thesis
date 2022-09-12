module ReelayComponents {
    

    @ component for receiving Data which is then sent out as ReelayEvent
    passive component ReelaySensor {
        
        constant number_of_serial_ports = 10;

        @ Used by the rateGroup to set the sensor baudrate
        sync input port schedIn: Svc.Sched

        @ Port for setting the data channel names
        guarded input port setDataChannelName : DataChannelName

        @ Port for receiving the data
        guarded input port serialIn : [number_of_serial_ports] serial
        #sync input port serialIn: serial

        output port reelayEventOut : EventIn

        #@ Port for forwarding dataIn (TODO assert for each i is_connectedSerialIn(i) <-> has is_connectedSerialOut(i))
        #output port  serialOut : [number_of_serial_ports] serial
        #output port  serialOut : serial

        @ Port for time
        time get port timeGetOut

        @ Port for events
        event port eventOut

        @ Port for text events
        text event port textEventOut

        @ Telemetry port
        telemetry port tlmOut

        event NO_VALUE_PRESENT (
            port_number : U32
        ) severity activity low \
        format "No value present at port: {}"
        
        #this->log_ACTIVITY_HI_UNKNOWN_TYPE(port_type);
        event UNKNOWN_TYPE (
            t : ReelayDataType,
            p : U32
        ) severity activity high \
        format "Unknown Type {} at port {}"


        telemetry reelayEvent: string id 0
    }
}