#include <iostream>
#include <bits/stdc++.h>
#include <curl/curl.h>
#include <iostream>
#include <bits/stdc++.h>
#include <curl/curl.h>
#include <climits>
#include <thread>
#include <vector>
#include <future>
using namespace std;

bool isPalindrome(string S){
    string P = S;
    reverse(P.begin(), P.end());
    if (S == P) return true;
    else return false;
}

static size_t WriteCallback(char *contents, size_t size, size_t nmemb, char *buffer_in){
    ((std::string*)buffer_in)->append((char*)contents, size*nmemb);
    return size*nmemb;
}

string makeRequest(string start){
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    std::string token;
    if(curl){
      std::ostringstream oss;
      oss << "https://api.pi.delivery/v1/pi?start=" << start << "&numberOfDigits=1000&radix=10";
      std::string url = oss.str();
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      std::string token = readBuffer.substr(readBuffer.find(":") + 1);
      size_t start =  token.find_first_of ("\""),
             end =  token.find_last_of ("\"");
      token = token.substr(start+1,end-start-1);
      return token;
    }
}

void searchChunk(string aux, string Pi, int n,string start){
    int c = 0;
    while (c<Pi.size()-n){
        if (isPalindrome(aux)){
            std::cout << "\n Found a palimdrom  ->" << aux<<"\n";
            std::cout << "\n chunk  ->" << start <<"\n";
        }
        c++;
        aux = Pi.substr(c,n);
    }
}

void Search_PalPrime(long p_start){
  long start = p_start;
  int n = 15;
  long num_digits = 1000;
  long end = start + 10000000;

  while(start< end){
    std::string token = makeRequest(to_string(start));
    if(token.length()==1000){
      std::string aux = token.substr(0,n);
      searchChunk(aux,token,n,to_string(start));
      start=start+num_digits;
    }else{
      continue;
    }
  }


}
int main() {
  long interval = 1000000000;
  int num_tasks = 200;
  std::vector<std::future<void>> tasks;

    for(auto i = 0; i != num_tasks; ++i)
      tasks.emplace_back(std::async(std::launch::async, Search_PalPrime,i*interval));


    return 0;
  
}
