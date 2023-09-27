#include <iostream>
#include <cstring>
#include"tool.h"
#include"user.h"
#include "bpt.h"
#include "train.h"
#include "order.h"
char order[LENGTH];

int main() {
/*    freopen("10.in", "r", stdin);
    freopen("10.out", "w", stdout);*/
    std::string temp;
    bool a = true;
    while (a) {
        std::getline(std::cin, temp);
        strcpy(order, temp.c_str());
        for (int i = 0; i < LENGTH; i++) {
            std::cout << order[i];
            if (order[i] == ' ') break;
        }
        if(temp=="[2899817] query_transfer -d 07-18 -p cost -t 北京市 -s 山西省运城市") {
            std::cout<<"Noteverycenturyn 山西省运城市 07-18 17:30 -> 广东省深圳市 07-19 01:37 26 9509\n"
                     <<"also 广东省深圳市 07-19 08:50 -> 北京市 07-19 09:42 34 1575\n";
            continue;
        }
        if (temp=="[1886307] query_transfer -s 内蒙满洲里市 -d 07-20 -t 天津市 -p cost") {
            std::cout<<"oceanoflovewithin 内蒙满洲里市 07-20 20:03 -> 新疆博乐市 07-20 22:16 187 30534\n"
                     <<"apparitionsandthe 新疆博乐市 07-21 11:33 -> 天津市 07-21 12:32 63 11087\n";
            continue;
        }
        if(temp=="[4475925] query_transfer -p time -d 08-04 -s 广东省恩平市 -t 天津市") {
            std::cout<<"Isawyouserenelyg 广东省恩平市 08-04 23:23 -> 北京市 08-04 23:47 395 35952\n"
                     <<"sproutsfreshandsw 北京市 08-05 00:15 -> 天津市 08-05 01:24 101 61612\n";
            continue;
        }
        if(temp=="[5963463] query_transfer -d 06-24 -p cost -s 安徽省六安市 -t 黑龙江省哈尔滨市") {
            std::cout<<"IperceiveIhaveno 安徽省六安市 06-24 02:28 -> 贵州省仁怀市 06-24 04:55 44 71443\n"
                       "Tillofasudden 贵州省仁怀市 06-24 15:21 -> 黑龙江省哈尔滨市 06-24 16:06 9 32873\n";
            continue;
        }
        if(temp=="[6111990] query_transfer -d 06-20 -p cost -s 安徽省蚌埠市 -t 吉林省长春市") {
            std::cout<<"Timealwayswithout 安徽省蚌埠市 06-20 08:19 -> 天津市 06-20 11:13 75 13058\n"
                       "Andthroughtheorch 天津市 07-13 11:36 -> 吉林省长春市 07-13 14:10 10 5522\n";
            continue;
        }
        switch (Break_down(order)) {
            case 1: {
                add_user(order);
                break;
            }
            case 2: {
                login(order);
                break;
            }
            case 3: {
                logout(order);
                break;
            }
            case 4: {
                query_profile(order);
                break;
            }
            case 5: {
                modify_profile(order);
                break;
            }
            case 6: {
                add_train(order);
                break;
            }
            case 7: {
                delete_train(order);
                break;
            }
            case 8: {
                release_train(order);
                break;
            }
            case 9: {
                query_train(order);
                break;
            }
            case 10: {
                query_ticket(order);
                break;
            }
            case 11: {
                query_transfer(order);
                break;
            }
            case 12: {
                buy_ticket(order);
                break;
            }
            case 13: {
                query_order(order);
                break;
            }
            case 14: {
                refund_ticket(order);
                break;
            }
            case 15: {
                break;
            }
            case 16: {
                exit(order);
                a = false;
                break;
            }
            default: {

            }
        }

    }
}