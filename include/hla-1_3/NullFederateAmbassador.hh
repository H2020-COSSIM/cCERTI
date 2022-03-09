// HLA 1.3 Header "NullFederateAmbassador.hh"

#ifndef NullFederateAmbassador_h
#define NullFederateAmbassador_h

#include <RTI.hh>

class NullFederateAmbassador : public RTI::FederateAmbassador
{
public:
    NullFederateAmbassador() { }
    virtual ~NullFederateAmbassador() { }

    // Federation Management

    virtual void synchronizationPointRegistrationSucceeded(const char *)  { }
 
    virtual void synchronizationPointRegistrationFailed(const char *)  { }

    virtual void announceSynchronizationPoint(const char *, const char *)  { }

    virtual void federationSynchronized(const char *)  { }

    virtual void initiateFederateSave(const char *)  { }

    virtual void federationSaved() { }

    virtual void federationNotSaved() { }

    virtual void requestFederationRestoreSucceeded(const char *)  { }

    virtual void requestFederationRestoreFailed(const char *, const char *)  { }

    virtual void federationRestoreBegun() { }

    virtual void initiateFederateRestore(const char *, RTI::FederateHandle)  { }

    virtual void federationRestored(){ }

    virtual void federationNotRestored() { }

    // Declaration Management

    virtual void startRegistrationForObjectClass(RTI::ObjectClassHandle)  { }

    virtual void stopRegistrationForObjectClass(RTI::ObjectClassHandle)  { }

    virtual void turnInteractionsOn(RTI::InteractionClassHandle)  { }

    virtual void turnInteractionsOff(RTI::InteractionClassHandle)  { }

    // Object Management

    virtual void discoverObjectInstance(RTI::ObjectHandle , RTI::ObjectClassHandle , const char *)  { }

    virtual void reflectAttributeValues(RTI::ObjectHandle, const RTI::AttributeHandleValuePairSet &, 
					const RTI::FedTime &, const char *, RTI::EventRetractionHandle)  { }

    virtual void reflectAttributeValues(RTI::ObjectHandle, const RTI::AttributeHandleValuePairSet &, const char *)  { }

    virtual void receiveInteraction(RTI::InteractionClassHandle, const RTI::ParameterHandleValuePairSet &, 
				    const RTI::FedTime &, const char *, RTI::EventRetractionHandle)  { }

    virtual void receiveInteraction(RTI::InteractionClassHandle, const RTI::ParameterHandleValuePairSet &, 
				    const char *) { }

    virtual void removeObjectInstance(RTI::ObjectHandle, const RTI::FedTime &, const char *, 
				      RTI::EventRetractionHandle)  { }

    virtual void removeObjectInstance(RTI::ObjectHandle, const char *)  { }

    virtual void attributesInScope(RTI::ObjectHandle, const RTI::AttributeHandleSet &)  { }

    virtual void attributesOutOfScope(RTI::ObjectHandle, const RTI::AttributeHandleSet &)  { }

    virtual void provideAttributeValueUpdate(RTI::ObjectHandle, const RTI::AttributeHandleSet &)  { }

    virtual void turnUpdatesOnForObjectInstance(RTI::ObjectHandle, const RTI::AttributeHandleSet &)  { }

    virtual void turnUpdatesOffForObjectInstance(RTI::ObjectHandle, const RTI::AttributeHandleSet &) { }

    // Ownership Management

    virtual void requestAttributeOwnershipAssumption(RTI::ObjectHandle, const RTI::AttributeHandleSet &, 
						     const char *)  { }

    virtual void attributeOwnershipDivestitureNotification(RTI::ObjectHandle, const RTI::AttributeHandleSet &)  { }

    virtual void attributeOwnershipAcquisitionNotification(RTI::ObjectHandle, const RTI::AttributeHandleSet &)  { }

    virtual void attributeOwnershipUnavailable(RTI::ObjectHandle, const RTI::AttributeHandleSet &)  { }

    virtual void requestAttributeOwnershipRelease(RTI::ObjectHandle, const RTI::AttributeHandleSet &, 
						  const char *)  { }

    virtual void confirmAttributeOwnershipAcquisitionCancellation(RTI::ObjectHandle,
								  const RTI::AttributeHandleSet &)  { }

    virtual void informAttributeOwnership(RTI::ObjectHandle, RTI::AttributeHandle, RTI::FederateHandle)  { }

    virtual void attributeIsNotOwned(RTI::ObjectHandle, RTI::AttributeHandle) { }

    virtual void attributeOwnedByRTI(RTI::ObjectHandle, RTI::AttributeHandle) { }

    // Time Management

    virtual void timeRegulationEnabled(const RTI::FedTime &)  { }

    virtual void timeConstrainedEnabled(const RTI::FedTime &)  { }

    virtual void timeAdvanceGrant(const RTI::FedTime &)  { }

    virtual void requestRetraction(RTI::EventRetractionHandle)  { }
};

#endif
