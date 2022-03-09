// HLA 1.3 Header "RTIambServices.hh"

typedef FederateAmbassador *FederateAmbassadorPtr ;


/**
 * @defgroup HLA13_FederationManagement Federation Management
 * @ingroup  HLA13_RTI_Ambassador
 * @{
 */

/**
 * Create the federation execution.
 * @param[in] executionName, the name of the federation to be created
 * @param[in] FEDid, the file path to the Federation Execution Data file, a.k.a. FED file.
 *            this file can be an XML file or a "fed" file.
 * @exception FederationExecutionAlreadyExists the RTI has already registered a federation with the same name
 * @exception CouldNotOpenFED the FED (Federation Execution Data) file cannot be found or read properly (check RTIG log output)
 * @exception ErrorReadingFED
 * @exception ConcurrentAccessAttempted
 * @exception RTIinternalError
 */
void createFederationExecution(const char *executionName, const char *FEDid);

/**
 * Destroys the federation execution.
 * @param[in] executionName the name of the federation
 */
void destroyFederationExecution(const char *executionName);

/**
 * Joins the federation execution.
 * 
 */
FederateHandle joinFederationExecution(const char *, const char *, FederateAmbassadorPtr);

void resignFederationExecution(ResignAction);

/**
 * Register Federation Synchronization Point.
 * All federates who have joined the federation are informed.
 *
 * @param[in] label Synchronization point label
 * @param[in] tag User-supplied tag
 */
void registerFederationSynchronizationPoint(const char *, const char *);

/**
 * Register Federation Synchronization Point with set of federates.
 * Only federates in the set are informed.
 *
 * @param[in] label Synchronization point label
 * @param[in] tag User-supplied tag
 * @param[in] federates set of federate designators
 */
void registerFederationSynchronizationPoint(const char *label, const char *tag, const FederateHandleSet &federates);

void synchronizationPointAchieved(const char *);

void requestFederationSave(const char *, const FedTime &);

void requestFederationSave(const char *);

void federateSaveBegun();

void federateSaveComplete();

void federateSaveNotComplete();

void requestFederationRestore(const char *);

void federateRestoreComplete();

void federateRestoreNotComplete();

/** @} end group HLA13_FederationManagement */

/**
 * @defgroup HLA13_DeclarationManagement Declaration Management
 * @ingroup  HLA13_RTI_Ambassador
 * @{
 */

void publishObjectClass(ObjectClassHandle, const AttributeHandleSet &);

void unpublishObjectClass(ObjectClassHandle);

void publishInteractionClass(InteractionClassHandle);

void unpublishInteractionClass(InteractionClassHandle);

void subscribeObjectClassAttributes(ObjectClassHandle, const AttributeHandleSet &, Boolean = RTI_TRUE);

void unsubscribeObjectClass(ObjectClassHandle);

void subscribeInteractionClass(InteractionClassHandle, Boolean = RTI_TRUE);

void unsubscribeInteractionClass(InteractionClassHandle);

/** @} end group HLA13_DeclarationManagement */

/**
 * @defgroup HLA13_ObjectManagement Object Management
 * @ingroup  HLA13_RTI_Ambassador
 * @{
 */


ObjectHandle registerObjectInstance(ObjectClassHandle, const char *);

ObjectHandle registerObjectInstance(ObjectClassHandle);

/**
 * Update Attribute Values (with time) service (HLA 1.3, §6.4).
 * Provide current values to the federation for instance attributes owned by
 * the federate.
 * @param[in] object     Object instance designator
 * @param[in] attributes Set of attribute designator and value pairs
 * @param[in] tag        User supplied tag
 */
EventRetractionHandle updateAttributeValues(ObjectHandle, const AttributeHandleValuePairSet &,
					    const FedTime &, const char *);

/**
 * Update Attribute Values (without time) service (HLA 1.3, §6.4).
 * Provide current values to the federation for instance attributes owned by
 * the federate.
 * @param[in] object     Object instance designator
 * @param[in] attributes Set of attribute designator and value pairs
 * @param[in] tag        User supplied tag
 */
void updateAttributeValues(ObjectHandle object, const AttributeHandleValuePairSet& attributes, const char * tag);

/**
 * Send Interaction with time
 * This service (HLA 1.3) send an interaction into the federation.
 * As the federation time argument is supplied, an event retraction designator is returned.
 * @param interactionClassHandle Interaction class designator
 * @param parameters Set of interaction parameters designator and value pairs
 * @param time Federation time
 * @param tag User-supplied tag
 */
EventRetractionHandle sendInteraction(InteractionClassHandle interactionClassHandle, const ParameterHandleValuePairSet& parameters,
				      const FedTime& time, const char * tag);

void sendInteraction(InteractionClassHandle, const ParameterHandleValuePairSet &, const char *);

/**
 * Delete Object with time
 * This service (HLA 1.3, §6.8) deletes an object instance.
 * As the federation time argument is supplied, an event retraction designator is returned.
 * @param theObject Object designator
 * @param theTime Federation time
 * @param theTag user supplied tag
 */
EventRetractionHandle deleteObjectInstance(ObjectHandle, const FedTime &, const char *);

/**
 * Delete Object without time
 * This service (HLA 1.3) deletes an object instance.
 * @param theObject Object designator
 * @param theTag user supplied tag
 */
void deleteObjectInstance(ObjectHandle, const char *);

void localDeleteObjectInstance(ObjectHandle);

void changeAttributeTransportationType(ObjectHandle, const AttributeHandleSet &, TransportationHandle);

void changeInteractionTransportationType(InteractionClassHandle, TransportationHandle);

void requestObjectAttributeValueUpdate(ObjectHandle, const AttributeHandleSet &);

void requestClassAttributeValueUpdate(ObjectClassHandle, const AttributeHandleSet &);

/** @} end group HLA13_ObjectManagement */

/**
 * @defgroup HLA13_OwnershipManagement Ownership Management
 * @ingroup  HLA13_RTI_Ambassador
 * @{
 */

void unconditionalAttributeOwnershipDivestiture(ObjectHandle, const AttributeHandleSet &);

void negotiatedAttributeOwnershipDivestiture(ObjectHandle, const AttributeHandleSet &, const char *);

void attributeOwnershipAcquisition(ObjectHandle, const AttributeHandleSet &desiredAttributes, const char *);

void attributeOwnershipAcquisitionIfAvailable(ObjectHandle, const AttributeHandleSet &);

AttributeHandleSet *attributeOwnershipReleaseResponse(ObjectHandle, const AttributeHandleSet &);

void cancelNegotiatedAttributeOwnershipDivestiture(ObjectHandle, const AttributeHandleSet &);

void cancelAttributeOwnershipAcquisition(ObjectHandle, const AttributeHandleSet &);

void queryAttributeOwnership(ObjectHandle, AttributeHandle);

Boolean isAttributeOwnedByFederate(ObjectHandle, AttributeHandle);

/** @} end group HLA13_OwnershipManagement */

/**
 * @defgroup HLA13_TimeManagement Time Management
 * @ingroup  HLA13_RTI_Ambassador
 * @{
 */


void enableTimeRegulation(const FedTime &, const FedTime &);

void disableTimeRegulation();

void enableTimeConstrained();

void disableTimeConstrained();

void timeAdvanceRequest(const FedTime &);

void timeAdvanceRequestAvailable(const FedTime &);

void nextEventRequest(const FedTime &);

void nextEventRequestAvailable(const FedTime &);

void flushQueueRequest(const FedTime &);

void enableAsynchronousDelivery();

void disableAsynchronousDelivery();

void queryLBTS(FedTime &);

void queryFederateTime(FedTime &);

void queryMinNextEventTime(FedTime &);

void modifyLookahead(const FedTime &);

void queryLookahead(FedTime &);

void retract(EventRetractionHandle theHandle);

void changeAttributeOrderType(ObjectHandle, const AttributeHandleSet &, OrderingHandle);

void changeInteractionOrderType(InteractionClassHandle, OrderingHandle);

/** @} end group HLA13_TimeManagement */

/**
 * @defgroup HLA13_DataDistributionManagement Data Distribution Management
 * @ingroup  HLA13_RTI_Ambassador
 * @{
 */


Region *createRegion(SpaceHandle, ULong);

void notifyAboutRegionModification(Region &theRegion);

void deleteRegion(Region *);

RTI::ObjectHandle registerObjectInstanceWithRegion(ObjectClassHandle, const char *, AttributeHandle [],
					      Region *theRegions[], ULong);

RTI::ObjectHandle registerObjectInstanceWithRegion(ObjectClassHandle, AttributeHandle [], Region *regions[], ULong);

void associateRegionForUpdates(Region &, ObjectHandle, const AttributeHandleSet &);

void unassociateRegionForUpdates(Region &, ObjectHandle);

void subscribeObjectClassAttributesWithRegion(ObjectClassHandle, Region &, const AttributeHandleSet &,
					      Boolean = RTI_TRUE);

void unsubscribeObjectClassWithRegion(ObjectClassHandle, Region &);

void subscribeInteractionClassWithRegion(InteractionClassHandle, Region &, Boolean = RTI_TRUE);

void unsubscribeInteractionClassWithRegion(InteractionClassHandle, Region &);

EventRetractionHandle sendInteractionWithRegion(InteractionClassHandle, const ParameterHandleValuePairSet &,
						const FedTime &, const char *, const Region &);

void sendInteractionWithRegion(InteractionClassHandle, const ParameterHandleValuePairSet &,
			       const char *, const Region &);

void requestClassAttributeValueUpdateWithRegion(ObjectClassHandle, const AttributeHandleSet &, const Region &);

/** @} end group HLA13_DataDistributionManagement */

/**
 * @defgroup HLA13_SupportService Support Service
 * @ingroup  HLA13_RTI_Ambassador
 * @{
 */

ObjectClassHandle getObjectClassHandle(const char *);

char *getObjectClassName(ObjectClassHandle);

AttributeHandle getAttributeHandle(const char *, ObjectClassHandle);

char *getAttributeName(AttributeHandle, ObjectClassHandle);

InteractionClassHandle getInteractionClassHandle(const char *);

char *getInteractionClassName(InteractionClassHandle);

ParameterHandle getParameterHandle(const char *, InteractionClassHandle);

char *getParameterName(ParameterHandle, InteractionClassHandle);

ObjectHandle getObjectInstanceHandle(const char *);

char *getObjectInstanceName(ObjectHandle);

SpaceHandle getRoutingSpaceHandle(const char *);

char *getRoutingSpaceName(SpaceHandle);

RTI::DimensionHandle getDimensionHandle(const char *, SpaceHandle);

char *getDimensionName(DimensionHandle, SpaceHandle);

RTI::SpaceHandle getAttributeRoutingSpaceHandle(AttributeHandle, ObjectClassHandle);

RTI::ObjectClassHandle getObjectClass(ObjectHandle);

RTI::SpaceHandle getInteractionRoutingSpaceHandle(InteractionClassHandle);

TransportationHandle getTransportationHandle(const char *);

char *getTransportationName(TransportationHandle);

OrderingHandle getOrderingHandle(const char *);

char *getOrderingName(OrderingHandle);

/**
 * Sets the ClassRelevanceAdvisory (CRA) switch to true. The switch
 * state is hold on the RTIG side. That's why the message
 * ENABLE_CLASS_RELEVANCE_ADVISORY_SWITCH
 * is transmitted to RTIA. RTIA transmits the message towards RTIG.
 *
 * By default, the CRA switch is true. This causes a delivery of the
 * federate service startRegistrationForObjectClass to a publisher
 * if there are any new subscribers for the federates published object
 * classes. If there are no more subscribers a publisher gets the
 * federate service stopRegistrationForObjectClass.
 *
 * By disabling the CRA switch the federate is no longer informed by
 * subscriptions to its published object classes, i.e. the federate
 * services startRegistrationForObjectClass and
 * stopRegistrationForObjectClass respectively are not invoked.
 * @see disableClassRelevanceAdvisorySwitch()
 */
void enableClassRelevanceAdvisorySwitch();

/**
 * Sets the ClassRelevanceAdvisory (CRA) switch to false. The switch
 * state is hold on the RTIG side. That's why the message
 * DISABLE_CLASS_RELEVANCE_ADVISORY_SWITCH
 * is transmitted to RTIA. RTIA transmits the message towards RTIG.
 *
 * By default, the CRA switch is true. This causes a delivery of the
 * federate service startRegistrationForObjectClass to a publisher
 * if there are any new subscribers for the federates published object
 * classes. If there are no more subscribers a publisher gets the
 * federate service stopRegistrationForObjectClass.
 * @see enableClassRelevanceAdvisorySwitch()
 *
 * By disabling the CRA switch the federate is no longer informed by
 * subscriptions to its published object classes, i.e. the federate
 * services startRegistrationForObjectClass and
 * stopRegistrationForObjectClass respectively are not invoked.
 */
void disableClassRelevanceAdvisorySwitch();

/**
 * Sets the AttributeRelevanceAdvisory (ARA) switch to true. The switch
 * state is hold on the RTIG side. That's why the message
 * ENABLE_ATTRIBUTE_RELEVANCE_ADVISORY_SWITCH
 * is transmitted to RTIA. RTIA transmits the message towards RTIG.
 *
 * By default, the ARA switch is false. When enabling the ARA switch
 * the federate is informed by the federate service
 * turnUpdatesOnForObjectInstance of new object instances within remote
 * federates actively subscribed to its published attributes. If there
 * are no active subscribers for a set of instance-attributes the federate
 * receives the federate service turnUpdatesOffForObjectInstance.
 *
 * By disabling the ARA switch the federate is no longer informed by
 * subscriptions to its published attributes, i.e. the federate
 * services turnUpdatesOnForObjectInstance and
 * turnUpdatesOffForObjectInstance respectively are not invoked.
 * @see disableAttributeRelevanceAdvisorySwitch()
 */
void enableAttributeRelevanceAdvisorySwitch();

/**
 * Sets the AttributeRelevanceAdvisory (ARA) switch to false. The switch
 * state is hold on the RTIG side. That's why the message
 * DISABLE_ATTRIBUTE_RELEVANCE_ADVISORY_SWITCH
 * is transmitted to RTIA. RTIA transmits the message towards RTIG.
 *
 * By default, the ARA switch is false. When enabling the ARA switch
 * the federate is informed by the federate service
 * turnUpdatesOnForObjectInstance of new object instances within remote
 * federates actively subscribed to its published attributes. If there
 * are no active subscribers for a set of instance-attributes the federate
 * receives the federate service turnUpdatesOffForObjectInstance.
 * @see enableAttributeRelevanceAdvisorySwitch()
 *
 * By disabling the ARA switch the federate is no longer informed by
 * subscriptions to its published attributes, i.e. the federate
 * services turnUpdatesOnForObjectInstance and
 * turnUpdatesOffForObjectInstance respectively are not invoked.
 */
void disableAttributeRelevanceAdvisorySwitch();

/**
 * Sets the AttributeScopeAdvisory (ASA) switch to true. The switch state
 * is hold on the RTIG side. That's why the message
 * ENABLE_ATTRIBUTE_SCOPE_ADVISORY_SWITCH
 * is transmitted to RTIA. RTIA transmits the message towards RTIG.
 *
 * By default, the ASA switch is false. When enabling the ASA switch
 * the federate is informed by the federate services
 * attributesInScope and attributesOutScope respectively of discovered
 * or registrated but not owned attribute-instances intersecting or
 * leaving its subscription regions.
 *
 * By disabling the ASA switch the federate is no longer informed of
 * changes in attribute-instance scope, i.e. the federate
 * services attributesInScope and attributesOutScope respectively are
 * not invoked.
 * @see disableAttributeScopeAdvisorySwitch()
 */
void enableAttributeScopeAdvisorySwitch();

/**
 * Sets the AttributeScopeAdvisory (ASA) switch to false. The switch state
 * is hold on the RTIG side. That's why the message
 * DISABLE_ATTRIBUTE_SCOPE_ADVISORY_SWITCH
 * is transmitted to RTIA. RTIA transmits the message towards RTIG.
 *
 * By default, the ASA switch is false. When enabling the ASA switch
 * the federate is informed by the federate services
 * attributesInScope and attributesOutScope respectively of discovered
 * or registrated but not owned attribute-instances intersecting or
 * leaving its subscription regions.
 * @see enableAttributeScopeAdvisorySwitch()
 *
 * By disabling the ASA switch the federate is no longer informed of
 * changes in attribute-instance scope, i.e. the federate
 * services attributesInScope and attributesOutScope respectively are
 * not invoked.
 */
void disableAttributeScopeAdvisorySwitch();

/**
 * Sets the InteractionRelevanceAdvisory (IRA) switch to true. The switch
 * state is hold on the RTIG side. That's why the message
 * ENABLE_INTERACTION_RELEVANCE_ADVISORY_SWITCH
 * is transmitted to RTIA. RTIA transmits the message towards RTIG.
 *
 * By default, the IRA switch is true. This causes a delivery of the
 * federate service turnInteractionsOn to a publisher if there are
 * any new subscribers for the federates published interaction
 * classes. If there are no more subscribers a publisher gets the
 * federate service turnInteractionsOff().
 *
 * By disabling the IRA switch the federate is no longer informed by
 * subscriptions to its published interaction classes, i.e. the federate
 * services turnInteractionsOn and turnInteractionsOff respectively are
 * not invoked.
 * @see disableInteractionRelevanceAdvisorySwitch()
 */
void enableInteractionRelevanceAdvisorySwitch();

/**
 * Sets the InteractionRelevanceAdvisory (IRA) switch to false. The switch
 * state is hold on the RTIG side. That's why the message
 * DISABLE_INTERACTION_RELEVANCE_ADVISORY_SWITCH
 * is transmitted to RTIA. RTIA transmits the message towards RTIG.
 *
 * By default, the IRA switch is true. This causes a delivery of the
 * federate service turnInteractionsOn to a publisher if there are
 * any new subscribers for the federates published interaction
 * classes. If there are no more subscribers a publisher gets the
 * federate service turnInteractionsOff().
 * @see enableInteractionRelevanceAdvisorySwitch()
 *
 * By disabling the IRA switch the federate is no longer informed by
 * subscriptions to its published interaction classes, i.e. the federate
 * services turnInteractionsOn and turnInteractionsOff respectively are
 * not invoked.
 */
void disableInteractionRelevanceAdvisorySwitch();

/**
 * Evoke callback (HLA1.3).
 * Non-blocking callback evocation, returns immediately with or without 1 callback.
 * If CERTI has been compiled in HLA13NG_LIBRTI mode then tick
 * may evoke 0 or multiple callbacks (may even be infinite).
 * If CERTI has been compiled in LEGACY_LIBRTI mode then tick
 * may evoke 0 or at most 1 callback.
 * @return Always returns false.
 */
Boolean tick();

/**
 * Blocking callback evocation (CERTI extension).
 * Blocks until a callback delivery and then evokes a single callback.
 * Waits until a callback delivery, be careful.
 * @return true if additional callbacks pending, false otherwise
 * @warning This is a non-standard extension of the HLA 1.3/IEEE-1516 API.
 */
Boolean tick2();

/**
 * Generic callback evocation (CERTI extension).
 * Blocks up to "minimum" seconds until a callback delivery
 * and then evokes either single (multiple==false) or
 * multiple (multiple==true) callback(s).
 * @param[in] multiple if false will only evoke a single callback
 *                     if true may evoke multiple
 * @param[in] minimum the minimum amount of time spent in the tick call.
 * @param[in] maximum the maximum amount of time spent in the tick call.
 * @return true if additional callbacks pending, false otherwise
 * @warning This is a non-standard extension of the HLA 1.3/IEEE-1516 API.
 */
Boolean __tick_kernel(Boolean multiple, TickTime minimum, TickTime maximum);

/**
 * Evoke multiple callbacks (HLA1.3).
 * Blocks up to "minimum" seconds until a callback delivery, then evokes
 * multiple callbacks until no callback is pending, or until "maximum"
 * duration is reached.
 * @param[in] minimum the minimum amount of time spent in the tick call.
 * @param[in] maximum the maximum amount of time spent in the tick call.
 * @return true if additional callbacks pending, false otherwise
 */
Boolean tick(TickTime minimum, TickTime maximum);

/** @} end group HLA13_SupportService */

#ifdef CERTI_REALTIME_EXTENSIONS
/**
 * @defgroup CERTI_RealtimeManagement Real-time Management
 * @ingroup  HLA13_RTI_Ambassador
 * @{
 */

/** 
 * Set Priority for RTIA Process (CERTI Real-time extension).
 * @param priority the priority needed for RTIA process (from 0 to 99 on linux system)
 * @param schedPolicy the scheduling policy needed for RTIA process
 *           - SCHED_FIFO or SCHED_RR on Linux system
 * @warning This is a non-standard extension of the HLA 1.3 API.
 */
void setPriorityforRTIAProcess(int priority, unsigned int schedPolicy) ;

/** 
 * Set Processor Mask affinity for RTIA Process (CERTI Real-time extension).
 * @param mask the mask to be applied for RTIA process (depend on available number of processors)
 * @warning This is a non-standard extension of the HLA 1.3 API.
 */
void setAffinityforRTIAProcess(cpu_set_t mask) ;

/** @} end group CERTI_RealtimeManagement */
#endif

RTIambassador();

~RTIambassador();

RegionToken getRegionToken(Region *);

Region *getRegion(RegionToken);
