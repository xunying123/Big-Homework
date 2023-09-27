
#ifndef TICKET_SYSTEM_TOOL_H
#define TICKET_SYSTEM_TOOL_H

const int LENGTH = 50000;

int Break_down(const char *order) {
    int i = 0;
    for (; i < LENGTH; i++) {
        if (order[i] == ' ') break;
    }
    if (order[i + 1] == 'a' && order[i + 5] == 'u') return 1;
    if (order[i + 1] == 'l' && order[i + 4] == 'i') return 2;
    if (order[i + 1] == 'l' && order[i + 4] == 'o') return 3;
    if (order[i + 1] == 'q' && order[i + 11] == 'i') return 4;
    if (order[i + 1] == 'm' && order[i + 8] == 'p') return 5;
    if (order[i + 1] == 'a' && order[i + 5] == 't') return 6;
    if (order[i + 1] == 'd' && order[i + 8] == 't') return 7;
    if (order[i + 1] == 'r' && order[i + 9] == 't') return 8;
    if (order[i + 1] == 'q' && order[i + 11] == 'n') return 9;
    if (order[i + 1] == 'q' && order[i + 11] == 'e') return 10;
    if (order[i + 1] == 'q' && order[i + 11] == 's') return 11;
    if (order[i + 1] == 'b' && order[i + 5] == 't') return 12;
    if (order[i + 1] == 'q' && order[i + 11] == 'r') return 13;
    if (order[i + 1] == 'r' && order[i + 9] == 'i') return 14;
    if (order[i + 1] == 'c') return 15;
    if (order[i + 1] == 'e') return 16;
    return 0;
}

int calendar(int t) {
    int d1 = t % 100, m1 = t / 100;
    if (d1 == 29) {
        if (m1 == 2) {
            d1 = 1;
            m1++;
        }
    }
    if (d1 == 30) {
        if (m1 == 2) {
            d1 = 2;
            m1++;
        }
    }
    if (d1 == 31) {
        if (m1 == 2) {
            d1 = 3;
            m1++;
        }
        if ((m1 == 4) || (m1 == 6) || (m1 == 9) || (m1 == 11)) {
            d1 = 1;
            m1++;
        }
    }
    if (d1 > 31) {
        if (m1 == 2) {
            d1 = d1 - 28;
            m1++;
        } else if ((m1 == 4) || (m1 == 6) || (m1 == 9) || (m1 == 11)) {
            d1 = d1 - 30;
            m1++;
        } else {
            d1 = d1 - 31;
            m1++;
        }
    }
    return d1+m1*100;
}

int back(int a,int b) {
    int aa=a%100-b;
    if(aa<=0) {
        int bb=a/100;
        if(bb==3) {
            aa=28+aa;
            return a/100*100-100+aa;
        }
        if((bb == 5) || (bb == 7) || (bb == 10) || (bb == 12)) {
            aa=30+aa;
            return a/100*100-100+aa;
        }
        aa=31+aa;
        return a/100*100-100+aa;
    }
    else return a-b;
}

int cha (int a,int b) {
    int ans=0;
    int d1,d2,h1,h2,f1,f2,m1,m2;
    m1=a/1000000;
    m2=b/1000000;
    d1=a/10000%100;
    d2=b/10000%100;
    h1=a/100%100;
    h2=b/100%100;
    f1=a%100;
    f2=b%100;
    if(f1<=f2) {
        ans+=f2-f1;
    } else {
        ans+=f2+60-f1;
        h2--;
    }
    if(h1<=h2) {
        ans+=(h2-h1)*60;
    } else {
        ans+=(h2+24-h1)*60;
        d2--;
    }
    if(d1<=d2) {
        ans+=(d2-d1)*24*60;
    }
    else {
        ans-=(d1-d2)*24*60;
    }
    for(;m1<m2;m1++) {
        if (m1 == 2) {
            ans+=28*24*60;
        } else if ((m1 == 4) || (m1 == 6) || (m1 == 9) || (m1 == 11)) {
            ans+=30*24*60;
        } else {
            ans+=31*24*60;
        }

    }
    return ans;
}

void exit(char* order) {
    std::cout<<"bye"<<'\n';
}

void out(int k) {
    if(k>=10) std::cout<<k;
    else std::cout<<0<<k;
}

#endif //TICKET_SYSTEM_TOOL_H
