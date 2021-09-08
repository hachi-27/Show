#include <fstream>
#include "ccapi_cpp/ccapi_session.h"
std::vector<std::string> coins = {"requsdt", "snxusdt", "keepusdt", "dntusdt", "xvsusdt", "thetausdt", "funusdt", "icxusdt", "nulsusdt"};

namespace ccapi {
class MyEventHandler : public EventHandler {
public:
       int NumberOfSymbols = coins.size();
  explicit MyEventHandler(const std::string& filename, const std::map<std::string, std::string>& symbolByCorrelationIdMap)
      : f(filename), symbolByCorrelationIdMap(symbolByCorrelationIdMap) {
    f << "timestamp,symbol,bid_0_price,bid_1_price,bid_2_price,bid_3_price,bid_4_price,bid_5_price,bid_6_price,bid_7_price,bid_8_price,bid_9_price,bid_0_quantity,bid_1_quantity,bid_2_quantity,bid_3_quantity,bid_4_quantity,bid_5_quantity,bid_6_quantity,bid_7_quantity,bid_8_quantity,bid_9_quantity,ask_0_price,ask_1_price,ask_2_price,ask_3_price,ask_4_price,ask_5_price,ask_6_price,ask_7_price,ask_8_price,ask_9_price,ask_0_quantity,ask_1_quantity,ask_2_quantity,ask_3_quantity,ask_4_quantity,ask_5_quantity,ask_6_quantity,ask_7_quantity,ask_8_quantity,ask_9_quantity \n";
         f.flush();
  }
  bool processEvent(const Event& event, Session* session) override {
          std::string UPSYM;
          if(count <= (NumberOfSymbols )){
        std::cout<<"If condition"<<std::endl;
            if (event.getType() == Event::Type::SUBSCRIPTION_DATA) {
        count +=1;
        f.open ("hello.csv", std::ofstream::out | std::ofstream::app);
      for (const auto& message : event.getMessageList()) {
        auto messageTime = message.getTime();
        auto messageTimePair = UtilTime::divide(messageTime);
        auto aCorrelationId = message.getCorrelationIdList()[0];
        buffer.clear();
        buffer_1.clear();
        auffer.clear();
        auffer_1.clear();
        std::vector<ccapi::Element> elem = message.getElementList();
        buffer += std::to_string(messageTimePair.first);
        buffer += ",";
        UPSYM = symbolByCorrelationIdMap.at(aCorrelationId);
        std::cout<<UPSYM<<std::endl;
        for(auto&c : UPSYM) {c = toupper(c);}
        buffer += UPSYM;
        buffer += ",";
        //auto mid = elem.begin() + elem.size()/2;
       // for (const auto& element : message.getElementList()) {
        for(auto it = std::begin(elem), mid= std::begin(elem) + elem.size()/2; it != std::end(elem) && mid != std::end(elem) ; it++, mid++){

          const std::map<std::string, std::string>& elementNameValueMap = (*it).getNameValueMap();
          const std::map<std::string, std::string>& elementNameValueMap1 = (*mid).getNameValueMap();
          CCAPI_LOGGER_TRACE("elementNameValueMap = " + toString(elementNameValueMap));
          if (elementNameValueMap.find(CCAPI_BEST_BID_N_PRICE) != elementNameValueMap.end()) {
            buffer += elementNameValueMap.at(CCAPI_BEST_BID_N_PRICE);
            buffer += ",";
            buffer_1 += elementNameValueMap.at(CCAPI_BEST_BID_N_SIZE);
            buffer_1 += ",";
          }


          if (elementNameValueMap1.find(CCAPI_BEST_ASK_N_PRICE) != elementNameValueMap1.end()) {
            auffer += elementNameValueMap1.at(CCAPI_BEST_ASK_N_PRICE);
            auffer += ",";
            auffer_1 += elementNameValueMap1.at(CCAPI_BEST_ASK_N_SIZE);
            auffer_1 += ",";
          }
        }
        buffer = buffer + buffer_1 + auffer + auffer_1;
        buffer.pop_back();
        buffer += "\n";
        f << buffer;
        f.flush();
        f.close();
      }
    }
    }
     else{
 if (event.getType() == Event::Type::SUBSCRIPTION_DATA) {
        std::cout<<"Else condition"<< std::endl<<count<<std::endl;
       count = 1;
//      f.clear();
        f.open("hello.csv", std::ofstream::out | std::ofstream::trunc);
//      f.close();
      for (const auto& message : event.getMessageList()) {
        auto messageTime = message.getTime();
        auto messageTimePair = UtilTime::divide(messageTime);
        auto aCorrelationId = message.getCorrelationIdList()[0];
        buffer.clear();
        buffer_1.clear();
        auffer.clear();
        auffer_1.clear();
        buffer += "timestamp,symbol,bid_0_price,bid_1_price,bid_2_price,bid_3_price,bid_4_price,bid_5_price,bid_6_price,bid_7_price,bid_8_price,bid_9_price,bid_0_quantity,bid_1_quantity,bid_2_quantity,bid_3_quantity,bid_4_quantity,bid_5_quantity,bid_6_quantity,bid_7_quantity,bid_8_quantity,bid_9_quantity,ask_0_price,ask_1_price,ask_2_price,ask_3_price,ask_4_price,ask_5_price,ask_6_price,ask_7_price,ask_8_price,ask_9_price,ask_0_quantity,ask_1_quantity,ask_2_quantity,ask_3_quantity,ask_4_quantity,ask_5_quantity,ask_6_quantity,ask_7_quantity,ask_8_quantity,ask_9_quantity \n";
         f.flush();
        std::vector<ccapi::Element> elem = message.getElementList();
        buffer += std::to_string(messageTimePair.first);
        buffer += ",";
        UPSYM = symbolByCorrelationIdMap.at(aCorrelationId);
        std::cout<<UPSYM<<std::endl;
        for(auto & c : UPSYM) {c = toupper(c);}
        buffer += UPSYM;
        buffer += ",";
        //auto mid = elem.begin() + elem.size()/2;
       // for (const auto& element : message.getElementList()) {
        for(auto it = std::begin(elem), mid= std::begin(elem) + elem.size()/2; it != std::end(elem) && mid != std::end(elem) ; it++, mid++){

          const std::map<std::string, std::string>& elementNameValueMap = (*it).getNameValueMap();
          const std::map<std::string, std::string>& elementNameValueMap1 = (*mid).getNameValueMap();
          CCAPI_LOGGER_TRACE("elementNameValueMap = " + toString(elementNameValueMap));
          if (elementNameValueMap.find(CCAPI_BEST_BID_N_PRICE) != elementNameValueMap.end()) {
            buffer += elementNameValueMap.at(CCAPI_BEST_BID_N_PRICE);
            buffer += ",";
            buffer_1 += elementNameValueMap.at(CCAPI_BEST_BID_N_SIZE);
            buffer_1 += ",";
          }


          if (elementNameValueMap1.find(CCAPI_BEST_ASK_N_PRICE) != elementNameValueMap1.end()) {
            auffer += elementNameValueMap1.at(CCAPI_BEST_ASK_N_PRICE);
            auffer += ",";
            auffer_1 += elementNameValueMap1.at(CCAPI_BEST_ASK_N_SIZE);
            auffer_1 += ",";
          }
        }
        buffer = buffer + buffer_1 + auffer + auffer_1;
        buffer.pop_back();
        buffer += "\n";
        f << buffer;
        f.flush();
        f.close();
      }
    }
    }

    return true;
  }

 private:
  std::ofstream f;
  std::string buffer, buffer_1, auffer, auffer_1;
  std::map<std::string, std::string> symbolByCorrelationIdMap;
  int count = 1;
};
} /* namespace ccapi */
using ::ccapi::MyEventHandler;
using ::ccapi::Session;
using ::ccapi::SessionConfigs;
using ::ccapi::SessionOptions;
using ::ccapi::Subscription;
using ::ccapi::toString;
int main(int argc, char** argv) {
  std::map<std::string, std::string> symbolByCorrelationIdMap;
  int correlationId = 0;
  std::vector<Subscription> subscriptionList;
  std::string subscriptionOptions =
      std::string(CCAPI_MARKET_DEPTH_MAX) + "=10&" + CCAPI_CONFLATE_INTERVAL_MILLISECONDS + "=3000&" + CCAPI_CONFLATE_GRACE_PERIOD_MILLISECONDS + "=0";
  for(auto &elem : coins){
   Subscription subscription("binance", elem, "MARKET_DEPTH", subscriptionOptions, std::to_string(correlationId));
   symbolByCorrelationIdMap[std::to_string(correlationId)]  = elem;
   correlationId++;
   subscriptionList.push_back(subscription);
}
  SessionOptions sessionOptions;
  SessionConfigs sessionConfigs;
  MyEventHandler eventHandler("hello.csv", symbolByCorrelationIdMap);
  Session session(sessionOptions, sessionConfigs, &eventHandler);
  session.subscribe(subscriptionList);
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(INT_MAX));
  }
  session.stop();
  std::cout << "Bye" << std::endl;
  return EXIT_SUCCESS;
}
