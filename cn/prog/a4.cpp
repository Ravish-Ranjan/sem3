#include <random>
#include <utility>
#include <vector>

#define FRAME_LOSS_PROB 0.7
#define TOTAL_FRAMES 100

class Channel {
    private:
        std::mt19937 rand_dev;
        std::uniform_real_distribution<double> distrib;

    public:
        Channel(): rand_dev(std::random_device{}()), distrib(0.0,1.0) {}

        bool transmit(int frame_id) {
            bool is_success = distrib(rand_dev) >= FRAME_LOSS_PROB;
            if (!is_success) printf("%d,",frame_id);
            return is_success;
        }
};

std::vector<std::pair<std::string,double>> stop_and_wait(Channel* ch) {
    int total_count = 0, success_count = 0, failed_count = 0;

    printf("\nfailed attempt : ");
    
    for (int frame_id=1; frame_id<=TOTAL_FRAMES; frame_id++) {
        total_count++;
        
        if (ch->transmit(frame_id)) {
            success_count++;
        } else {
            failed_count++;
        }
    }

    printf("\n\n");

    std::vector<std::pair<std::string,double>> result;

    result.push_back({"total transmitted",total_count});
    result.push_back({"successful",success_count});
    result.push_back({"failed",failed_count});
    result.push_back({"re-transmitted",0});
    result.push_back({"loss-rate",(double)(failed_count*100) / total_count});
    result.push_back({"delivery-rate",(double)(success_count*100) / total_count});

    return result;
}

std::vector<std::pair<std::string,double>> stop_and_wait_arq(Channel* ch) {
    int total_count = 0, success_count = 0, failed_count = 0, re_transmisit = 0;

    printf("\nfailed attempt : ");

    for (int frame_id=1; frame_id<=TOTAL_FRAMES; frame_id++) {
        int attempts = 0;

        while (true) {
            attempts++;
            total_count++;
            if (ch->transmit(frame_id)) {
                success_count++;
                break;
            } else {
                re_transmisit++;
            }
        }
    }

    printf("\n\n");

    std::vector<std::pair<std::string,double>> result;

    result.push_back({"total transmitted",total_count});
    result.push_back({"successful",success_count});
    result.push_back({"failed",failed_count});
    result.push_back({"re-transmitted",re_transmisit});
    result.push_back({"loss-rate",(double)(failed_count*100) / total_count});
    result.push_back({"delivery-rate",(double)(success_count*100) / total_count});

    return result;
}

int main() {
    Channel* ch = new Channel();

    printf("---------------- Stop and Wait ----------------\n");
    
    auto snw_result = stop_and_wait(ch);

    for (const auto res:snw_result) {
        if (res.first == "loss-rate" || res.first == "delivery-rate")
            printf("%s : %.2f%\n",res.first.c_str(),res.second);
        else
            printf("%s : %.0f\n",res.first.c_str(),res.second);
    }

    printf("\n");

    printf("-------------- Stop and Wait ARQ --------------\n");
    
    auto snw_arq_result = stop_and_wait_arq(ch);

    for (const auto res:snw_arq_result) {
        if (res.first == "loss-rate" || res.first == "delivery-rate")
            printf("%s : %.2f %\n",res.first.c_str(),res.second);
        else
            printf("%s : %.0f\n",res.first.c_str(),res.second);    
    }
    
    return 0;
}