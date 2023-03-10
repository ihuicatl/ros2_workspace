// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*************************************************************************
 * @file OwnershipStrengthSubscriber.cpp
 * This file contains the implementation of the subscriber functions.
 *
 * This file was generated by the tool fastcdrgen.
 */

#include "OwnershipStrengthSubscriber.h"

#include <algorithm>

#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/participant/Participant.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/TopicDataType.h>

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

OwnershipStrengthSubscriber::OwnershipStrengthSubscriber()
    : mp_participant(nullptr)
    , mp_subscriber(nullptr)
{
}

OwnershipStrengthSubscriber::~OwnershipStrengthSubscriber()
{
    Domain::removeParticipant(mp_participant);
}

bool OwnershipStrengthSubscriber::init()
{
    ParticipantAttributes PParam;
    PParam.rtps.builtin.discovery_config.leaseDuration = c_TimeInfinite;
    PParam.rtps.setName("Participant_subscriber"); //You can put the name you want
    mp_participant = Domain::createParticipant(PParam);
    if (mp_participant == nullptr)
    {
        return false;
    }

    //Register the type

    Domain::registerType(mp_participant, (TopicDataType*) &myType);

    // Create Subscriber

    SubscriberAttributes Rparam;
    Rparam.topic.topicKind = NO_KEY;
    Rparam.topic.topicDataType = myType.getName(); //Must be registered before the creation of the subscriber
    Rparam.topic.topicName = "OwnershipStrengthPubSubTopic";
    Rparam.qos.m_reliability.kind = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;
    mp_subscriber = Domain::createSubscriber(mp_participant, Rparam, (SubscriberListener*)&m_listener);
    if (mp_subscriber == nullptr)
    {
        return false;
    }
    return true;
}

void OwnershipStrengthSubscriber::SubListener::onSubscriptionMatched(
        Subscriber* /*sub*/,
        MatchingInfo& info)
{
    if (info.status == MATCHED_MATCHING)
    {
        n_matched++;
        std::cout << "Subscriber matched" << std::endl;
    }
    else
    {
        n_matched--;
        std::cout << "Subscriber unmatched" << std::endl;

        // Must deregister here to make sure messages from weaker publishers are accepted.
        // It is also possible to deregister on a lost deadline, if applicable.
        m_hierarchy.DeregisterPublisher(info.remoteEndpointGuid);
    }
}

bool OwnershipStrengthSubscriber::StrengthHierarchy::IsMessageStrong(
        const ExampleMessage& st,
        const SampleInfo_t& info)
{
    unsigned int ownershipStrength = st.ownershipStrength();
    GUID_t guid = info.sample_identity.writer_guid();

    mapMutex.lock();

    // The strength-GUID pair is inserted in the strength hierarchy, if unique.
    strengthMap[ownershipStrength].insert(guid);

    // The set of strongest writers is extracted from the back of the map (highest strength)
    std::set<GUID_t>& strongestWriters = strengthMap.rbegin()->second;

    // The prioritised writer is arbitrarily chosen as the one with the lowest GUID, in case of a tie.
    const GUID_t& prioritisedStrongestWriter = *(strongestWriters.begin());
    bool strong = (guid == prioritisedStrongestWriter);

    mapMutex.unlock();

    if (!strong)
    {
        std::cout << "Weak message received and discarded (strength " << ownershipStrength << ")" << std::endl;
    }

    return strong;
}

void OwnershipStrengthSubscriber::StrengthHierarchy::DeregisterPublisher(
        GUID_t guid)
{
    mapMutex.lock();

    // We walk through the hierarchy and remove the publisher GUID
    for (auto& guidSet : strengthMap)
    {
        guidSet.second.erase(guid);
    }

    mapMutex.unlock();
}

void OwnershipStrengthSubscriber::SubListener::onNewDataMessage(
        Subscriber* sub)
{
    // Take data
    ExampleMessage st;

    if (sub->takeNextData(&st, &m_info) &&
            m_info.sampleKind == ALIVE &&
            m_hierarchy.IsMessageStrong(st, m_info))
    {
        // User message handling here, for a strong message
        ++n_msg;
        std::cout << "Message received with index " << n_msg << ", and strength " << st.ownershipStrength() \
                  << ", reading \"" << st.message() << "\"" << std::endl;
    }
}

void OwnershipStrengthSubscriber::run()
{
    std::cout << "Waiting for Data, press Enter to stop the Subscriber. " << std::endl;
    std::cin.ignore();
    std::cout << "Shutting down the Subscriber." << std::endl;
}
