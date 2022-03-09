// HLA 1.3 Header "federateAmbServices.hh"

// Federation Management

virtual void synchronizationPointRegistrationSucceeded(const char *)  = 0 ;

virtual void synchronizationPointRegistrationFailed(const char *)  = 0 ;

virtual void announceSynchronizationPoint(const char *, const char *)  = 0 ;

virtual void federationSynchronized(const char *)  = 0 ;

virtual void initiateFederateSave (const char *)  = 0 ;

virtual void federationSaved()  = 0 ;

virtual void federationNotSaved ()  = 0 ;

virtual void requestFederationRestoreSucceeded (const char *)  = 0 ;

virtual void requestFederationRestoreFailed(const char *, const char *)  = 0 ;

virtual void federationRestoreBegun()  = 0 ;

virtual void initiateFederateRestore (const char *, FederateHandle)  = 0 ;

virtual void federationRestored()  = 0 ;

virtual void federationNotRestored()  = 0 ;

// Declaration Management

virtual void startRegistrationForObjectClass(ObjectClassHandle)  = 0 ;

virtual void stopRegistrationForObjectClass(ObjectClassHandle)  = 0 ;

virtual void turnInteractionsOn(InteractionClassHandle)  = 0 ;

virtual void turnInteractionsOff(InteractionClassHandle) = 0 ;

// Object Management

virtual void discoverObjectInstance(ObjectHandle, ObjectClassHandle, const char *)  = 0 ;

virtual void reflectAttributeValues(ObjectHandle, const AttributeHandleValuePairSet &,
				    const FedTime &, const char *, EventRetractionHandle) = 0 ;

virtual void reflectAttributeValues(ObjectHandle, const AttributeHandleValuePairSet &, const char *)  = 0 ;

virtual void receiveInteraction(InteractionClassHandle, const ParameterHandleValuePairSet &,
				const FedTime &, const char *, EventRetractionHandle)  = 0 ;

virtual void receiveInteraction(InteractionClassHandle, const ParameterHandleValuePairSet &, const char *)  = 0 ;

virtual void removeObjectInstance(ObjectHandle, const FedTime &, const char *, EventRetractionHandle)  = 0 ;

virtual void removeObjectInstance(ObjectHandle, const char *)  = 0 ;

virtual void attributesInScope (ObjectHandle, const AttributeHandleSet &)  = 0 ;

virtual void attributesOutOfScope(ObjectHandle, const AttributeHandleSet &)  = 0 ;

virtual void provideAttributeValueUpdate(ObjectHandle, const AttributeHandleSet &)  = 0 ;

virtual void turnUpdatesOnForObjectInstance (ObjectHandle, const AttributeHandleSet &)  = 0 ;

virtual void turnUpdatesOffForObjectInstance (ObjectHandle, const AttributeHandleSet &)  = 0 ;

// Ownership Management

virtual void requestAttributeOwnershipAssumption(ObjectHandle, const AttributeHandleSet &, const char *)  = 0 ;

virtual void attributeOwnershipDivestitureNotification(ObjectHandle, const AttributeHandleSet &)  = 0 ;

virtual void attributeOwnershipAcquisitionNotification(ObjectHandle, const AttributeHandleSet &)  = 0 ;

virtual void attributeOwnershipUnavailable(ObjectHandle, const AttributeHandleSet &)  = 0 ;

virtual void requestAttributeOwnershipRelease(ObjectHandle, const AttributeHandleSet &, const char *)  = 0 ;

virtual void confirmAttributeOwnershipAcquisitionCancellation(ObjectHandle, const AttributeHandleSet &)  = 0 ;

virtual void informAttributeOwnership(ObjectHandle, AttributeHandle, FederateHandle)  = 0 ;

virtual void attributeIsNotOwned(ObjectHandle, AttributeHandle)  = 0 ;

virtual void attributeOwnedByRTI (ObjectHandle, AttributeHandle)  = 0 ;

// Time Management

virtual void timeRegulationEnabled(const  FedTime &)  = 0 ;

virtual void timeConstrainedEnabled(const FedTime &)  = 0 ;

virtual void timeAdvanceGrant(const FedTime &) = 0 ;

virtual void requestRetraction(EventRetractionHandle)  = 0 ;

virtual ~FederateAmbassador(){ ; }
