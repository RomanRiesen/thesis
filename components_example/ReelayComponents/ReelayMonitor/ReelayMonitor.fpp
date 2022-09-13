
module ReelayComponents {
    @ component for receiving ReelayEvents and then generating ReelayVerdicts from them

    @ TODO should potentiall be queue and then activated on a schedule set by a close-to-HW timer? Or RateGroups.
    passive component ReelayMonitor {

        @ Port for adding property
        sync input port newProperty : ReelayCreateProperty

        @ Port for removing property
        sync input port removeProperty : ReelayDeleteProperty

        @ Port for receiving the events
        sync input port eventIn : EventIn

        @ Port for sending verdicts
        output port verdictOut : ReelayVerdictOut

        @ Port for sending verdicts, but without any additional information
        output port verdictOutBool : BoolPort

        @ Port for time
        time get port timeGetOut

        @ Port for events
        event port eventOut

        @ Port for text events
        text event port textEventOut


        event NO_PROPERTY_REGISTERED() \
        severity activity low \
        format "No Property Registered."

        event INVALID_PROPERTY_STRING(property: ReelayProperty) \
        severity activity high \
        format "Tried to add invalid property {}"

        event INVALID_EVENT_STRING(reelay_event: ReelayEvent) \
        severity activity high \
        format "Tried to parse invalid event {}"

        event NEW_PROPERTY_REGISTERED (
            property : ReelayProperty
        ) severity activity low \
        format "New Property: {}"

        event ALL_PROPERTIES_POSITIVE () severity activity low \
        format "All properties are positive."

        event NEGATIVE_VERDICT (
            property : ReelayProperty
        ) severity activity high \
        format "FALSE -> {}"

        event PROPERTY_DELETED (
            property : ReelayProperty
        ) severity activity low \
        format "Deleted Property: {}"



    }
}