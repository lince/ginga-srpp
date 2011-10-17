
# set the include path found by configure
INCLUDES = -I/usr/local/include/directfb/ -I/usr/local/include/ginga/ \
	$(all_includes)

LIBS =

# the library search path.
localLibdir = /usr/local/lib/ginga

libncl30_so_LDFLAGS = -Wl,-soname,libncl30.so -shared \
-L/usr/local/lib -L/usr/local/lib/ginga
localLib_PROGRAMS = libncl30.so

libncl30_so_SOURCES = util/Comparator.cpp transition/Transition.cpp \
transition/TransitionBase.cpp transition/TransitionUtil.cpp \
time/flexibility/LinearTimeCostFunction.cpp \
time/flexibility/TemporalFlexibilityFunction.cpp switches/CompositeRule.cpp \
switches/DescriptorSwitch.cpp switches/Rule.cpp switches/RuleBase.cpp \
switches/SimpleRule.cpp switches/SwitchContent.cpp switches/SwitchNode.cpp \
reuse/ReferredNode.cpp reuse/ReferNode.cpp navigation/FocusDecoration.cpp \
navigation/KeyNavigation.cpp metainformation/Meta.cpp \
metainformation/Metadata.cpp link/Bind.cpp link/CausalLink.cpp link/Link.cpp \
layout/LayoutRegion.cpp layout/DeviceLayout.cpp \
layout/RegionBase.cpp interfaces/Anchor.cpp \
interfaces/CircleSpatialAnchor.cpp interfaces/ContentAnchor.cpp \
interfaces/IntervalAnchor.cpp interfaces/LabeledAnchor.cpp \
interfaces/LambdaAnchor.cpp interfaces/Port.cpp interfaces/PropertyAnchor.cpp \
interfaces/RectangleSpatialAnchor.cpp \
interfaces/RelativeTimeIntervalAnchor.cpp interfaces/SampleIntervalAnchor.cpp \
interfaces/SwitchPort.cpp interfaces/TextAnchor.cpp descriptor/Descriptor.cpp \
descriptor/DescriptorBase.cpp connectors/Action.cpp \
connectors/AssessmentStatement.cpp connectors/AttributeAssessment.cpp \
connectors/CausalConnector.cpp connectors/CompoundAction.cpp \
connectors/CompoundCondition.cpp connectors/CompoundStatement.cpp \
connectors/Connector.cpp connectors/ConnectorBase.cpp \
connectors/EventTransitionAssessment.cpp connectors/EventUtil.cpp \
connectors/Role.cpp connectors/SimpleAction.cpp \
connectors/SimpleCondition.cpp connectors/TriggerExpression.cpp \
connectors/ValueAssessment.cpp components/AbsoluteReferenceContent.cpp \
components/CompositeNode.cpp components/Content.cpp \
components/ContentNode.cpp components/ContentTypeManager.cpp \
components/ContextNode.cpp components/Node.cpp components/NodeEntity.cpp \
components/PrivateBase.cpp components/ReferenceContent.cpp \
components/RelativeReferenceContent.cpp animation/Animation.cpp \
Base.cpp Entity.cpp NclDocument.cpp Parameter.cpp

libncl30_so_LDADD = -ltelemidiautil -lpthread
