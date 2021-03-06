// eventpp library
// Copyright (C) 2018 Wang Qi (wqking)
// Github: https://github.com/wqking/eventpp
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "test.h"
#include "eventpp/utilities/eventutil.h"
#include "eventpp/eventdispatcher.h"


namespace {
void removeEd1(std::vector<int> * dataList)
{
	++(*dataList)[0];
}
void removeEd2(std::vector<int> * dataList)
{
	++(*dataList)[1];
}

// Use function pointer because it's comparable.
struct RemoveEdPolicies
{
	using Callback = void (*)(std::vector<int> * dataList);
};

} //unnamed namespace

TEST_CASE("eventutil, removeListener EventDispatcher")
{
	eventpp::EventDispatcher<int, void (std::vector<int> *), RemoveEdPolicies> dispatcher;
	constexpr int event = 3;
	
	std::vector<int> dataList(2);
	
	REQUIRE(dataList == std::vector<int> { 0, 0 });

	dispatcher.appendListener(event, &removeEd1);
	dispatcher.appendListener(event, &removeEd2);
	
	dispatcher.dispatch(event, &dataList);
	REQUIRE(dataList == std::vector<int> { 1, 1 });

	REQUIRE(removeListener(dispatcher, event, &removeEd1));
	dispatcher.dispatch(event, &dataList);
	REQUIRE(dataList == std::vector<int> { 1, 2 });

	REQUIRE(! removeListener(dispatcher, event, &removeEd1));

	REQUIRE(removeListener(dispatcher, event, &removeEd2));
	dispatcher.dispatch(event, &dataList);
	REQUIRE(dataList == std::vector<int> { 1, 2 });

	REQUIRE(! removeListener(dispatcher, event, &removeEd1));
	REQUIRE(! removeListener(dispatcher, event, &removeEd2));
}

TEST_CASE("eventutil, removeListener CallbackList")
{
	eventpp::CallbackList<void (std::vector<int> *), RemoveEdPolicies> callbackList;
	
	std::vector<int> dataList(2);
	
	REQUIRE(dataList == std::vector<int> { 0, 0 });

	callbackList.append(&removeEd1);
	callbackList.append(&removeEd2);
	
	callbackList(&dataList);
	REQUIRE(dataList == std::vector<int> { 1, 1 });

	REQUIRE(removeListener(callbackList, &removeEd1));
	callbackList(&dataList);
	REQUIRE(dataList == std::vector<int> { 1, 2 });

	REQUIRE(! removeListener(callbackList, &removeEd1));

	REQUIRE(removeListener(callbackList, &removeEd2));
	callbackList(&dataList);
	REQUIRE(dataList == std::vector<int> { 1, 2 });

	REQUIRE(! removeListener(callbackList, &removeEd1));
	REQUIRE(! removeListener(callbackList, &removeEd2));
}
