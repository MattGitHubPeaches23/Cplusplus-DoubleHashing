// Copyright (c) 2019, Matthew Chalifoux.

#include <iostream>
#include <iomanip>
#include <time.h>
#include <ctime>
using namespace std;
struct record{
    int _key;
    double _value;

    record(int k=0, double v=0);
    friend bool operator ==(const record& left, const record& right);
    friend ostream& operator <<(ostream& outs, const record& print_me);
};
struct LinkedList{
    record data;
    LinkedList();

    LinkedList *link;
};
struct store{
    store();
    LinkedList *connect;
};


class Timer{
private:
    clock_t _start;
    clock_t _stop;
public:
    Timer();
    void start(){
        _start = clock();
    }
    void stop(){
        _stop = clock();
    }
    double duration(){
        cout<<"["<<float(_stop)<<" - "<<float(_start)<<"] / "<<CLOCKS_PER_SEC<<endl;
        return double((_stop - _start)/CLOCKS_PER_SEC) ;
    }
};

template<typename record>
class open_hash{
    public:
        static const int SZ = 811;
        open_hash();
        record get_data(int index);
        int get_collisons();
        bool exists(int entry);
        void exists_Again(int entry, bool& maybe, int& index);
        void found(int entry, bool& maybe, int& index);
        void found_Again(int entry, bool& maybe, int& index);
        void remove_Again(int entry, int key, bool& maybe);
        bool remove(int entry);
        bool insert(const record& entry);
        void insert_again(const record& entry, int key, bool& maybe);
        void print(ostream& outs = cout) const;
        int get_size() const;
        bool check_Dup(const record& entry);
        void check_Dup_Again(const record& entry, int key, bool& maybe);
        template<typename h>
        friend ostream& operator <<(ostream& outs, const open_hash<h>& print_me);
    private:
        record data[SZ];
        int size;
        int collisons;
};

template<typename record>
class double_hash{
public:
    static const int SZ = 811;
    double_hash();
    record get_data(int index);
    int get_collisons();
    bool exists(int entry);
    void exists_Again(int entry, bool& maybe, int& index, int hash2);
    void found(int entry, bool& maybe, int& index);
    void found_Again(int entry, bool& maybe, int& index, int hash2);
    void remove_Again(int entry, int key, bool& maybe, int hash2);
    bool remove(int entry);
    bool insert(const record& entry);
    void insert_again(const record& entry, int key, bool& maybe, int hash2);
    void print(ostream& outs = cout) const;
    int get_size() const;
    bool check_Dup(const record& entry);
    void check_Dup_Again(const record& entry, int key, bool& maybe, int hash2);
    template<typename h>
    friend ostream& operator <<(ostream& outs, const double_hash<h>& print_me);
private:
    record data[SZ];
    int size;
    int collisons;
};

template<typename record>
class chained_hash{
    public:
        static const int SZ = 811;
        store get_data(int num);
        bool remove(int num);
        bool found(int num, record& sub);
        int get_size();
        chained_hash();
        bool exists(int num);
        bool exists_Change(const record& entry);
        bool insert(const record& entry);
        void print(ostream& outs = cout) const;
        void check_Dup(const record& entry);
        template<typename h>
        friend ostream& operator <<(ostream& outs, const chained_hash<h>& print_me);
    private:
        store front[SZ];
        int size;

};

template<typename record>
void test_hash_table_interactive(chained_hash<record>& open, string name);
template<typename record>
void test_hash_table_interactive(open_hash<record>& open, string name);
template<typename record>
void test_hash_table_random(open_hash<record> &open,int num, string name);
template<typename record>
void test_hash_table_random(chained_hash<record> &open,int num, string name);

void test_hash_table_interactive(double_hash<record>& open, string name);
void collision_test(int iterations = 700);
int main()
{

    const bool RANDOM_CHAINED = false;
    const bool RANDOM_OPEN = false;
    const bool INTERACTIVE_OPEN = false;
    const bool INTERACTIVE_CHAINED = false;
    const bool INTERACTIVE_DOUBLE_HASH = false;

    if (INTERACTIVE_OPEN){
        cout<<"-------  INTERACTIVE TESTS ---------------------------"<<endl;
        open_hash<record> open;
        test_hash_table_interactive(open, "open_hash_table");
    }

    if (INTERACTIVE_CHAINED){
        cout<<"-------  INTERACTIVE TESTS ---------------------------"<<endl;
        chained_hash<record> chained;
        test_hash_table_interactive(chained, "chained_hash_table");
    }

    if (RANDOM_CHAINED){
            //----------- RANDOM TEST ------------------------------
            //. . . . . .  Chained Hash Table . . . . . . . . . . .;
            chained_hash<record> c_table;
            test_hash_table_random(c_table, 2500, "chained_hash_table<record>");
            //cout<<c_table<<endl;
        }

    if (RANDOM_OPEN){
            //----------- RANDOM TEST ------------------------------
            //. . . . . .  Simple Hash Table . . . . . . . . . . .;
            open_hash<record> h_table;
            test_hash_table_random(h_table, 500, "open_hash_table<record>");
            //cout<<h_table<<endl;
            //cout << "collisions = " << h_table.get_collisons() << endl;
    }

    if(INTERACTIVE_DOUBLE_HASH == true){
        cout<<"-------  INTERACTIVE TESTS ---------------------------"<<endl;
        double_hash<record> open;
        test_hash_table_interactive(open, "double_hash_table");
    }

    collision_test();


    return 0;
}
Timer::Timer(){

}

template<typename record>
void test_hash_table_random(chained_hash<record> &open,int num, string name){
    cout << "**********************************************************" << endl;
    cout << "          RANDOM HASH TEST : " << name << "           " << endl;
    cout << "**********************************************************" << endl;

    int cat[num];
    for(int i = 0; i < num; ++i){
        int num0, num2;
        num0 = rand()%10000;
        num2 = rand()%10000;
        record sub(num0,num2);
        open.insert(sub);
        cat[i] = sub._key;
    }

    int found = 0;
    int not_found = 0;
    for(int i = 0; i < num/2; ++i){
        int num0;
        num0 = rand()%num;
        int bt = cat[num0];
        if(open.exists(bt) == true){
            found += 1;
        }
        else if(open.exists(bt)== false){
            not_found += 1;
        }
    }

    cout << "FOUND = " << found << endl;
    cout << "NOT FOUND = " << not_found << endl;
    cout << "*************************" << endl;

    found = 0;
    not_found = 0;

    int uio[num];

    for(int i = 0; i < num; ++i){
        bool loop = false;
        while(loop == false){
            int num0;
            num0 = rand()%10000;
            bool turn = false;
            for(int j = 0; j < open.SZ; ++j){
                store here;
                here = open.get_data(j);
                //cout << "here.connect.data._key = " << here.connect->data._key << endl;
                LinkedList *ptr;
                ptr = here.connect;
                while(ptr != NULL){
                    if(ptr->data._key == num0){
                        turn = true;
                    }
                    ptr = ptr->link;
                }
            }
            if(turn == false){
                uio[i] = num0;
                loop = true;
            }
        }
    }

    for(int i = 0; i < num/2; ++i){
        int num0;
        num0 = uio[i];
        if(open.exists(num0) == true){
            found += 1;
        }
        else if(open.exists(num0) == false){
            not_found += 1;
        }
    }

    cout << "FOUND = " << found << endl;
    cout << "NOT FOUND = " << not_found << endl;




//    int ten[num];
//    for(int i = 0; i < num; ++i){
//        bool loop = false;
//        while(loop == false){
//            int num0;
//            num0 = rand()%10000;
//            bool turn = false;
//            for(int j = 0; j < open.SZ; ++j){
//                record here;
//                here = open.get_data(j);
//                if(here._key == num0){
//                    turn = true;
//                }
//            }
//            if(turn == false){
//                ten[i] = num0;
//                loop = true;
//            }
//        }
//    }

//    found = 0;
//    not_found = 0;

//    for(int i = 0; i < num/2; ++i){
//        int num0;
//        num0 = ten[i];
//        if(open.exists(num0) == true){
//            found += 1;
//        }
//        else if(open.exists(num0) == false){
//            not_found += 1;
//        }
//    }

//    cout << "FOUND = " << found << endl;
//    cout << "NOT FOUND = " << not_found << endl;
}

template<typename record>
void test_hash_table_random(open_hash<record> &open,int num, string name){
    cout << "**********************************************************" << endl;
    cout << "          RANDOM HASH TEST : " << name << "           " << endl;
    cout << "**********************************************************" << endl;


    //record here[num];
    int cat[num];
    for(int i = 0; i < num; ++i){
        int num0, num2;
        num0 = rand()%10000;
        num2 = rand()%10000;
        record sub(num0,num2);
        open.insert(sub);
        cat[i] = sub._key;
    }

    int found = 0;
    int not_found = 0;
    for(int i = 0; i < num/2; ++i){
        int num0;
        num0 = rand()%num;
        int bt = cat[num0];
        if(open.exists(bt) == true){
            found += 1;
        }
        else if(open.exists(bt)== false){
            not_found += 1;
        }
    }

    cout << "FOUND = " << found << endl;
    cout << "NOT FOUND = " << not_found << endl;
    cout << "*************************" << endl;

    int ten[num];
    for(int i = 0; i < num; ++i){
        bool loop = false;
        while(loop == false){
            int num0;
            num0 = rand()%10000;
            bool turn = false;
            for(int j = 0; j < open.SZ; ++j){
                record here;
                here = open.get_data(j);
                if(here._key == num0){
                    turn = true;
                }
            }
            if(turn == false){
                ten[i] = num0;
                loop = true;
            }
        }
    }

    found = 0;
    not_found = 0;

    for(int i = 0; i < num/2; ++i){
        int num0;
        num0 = ten[i];
        if(open.exists(num0) == true){
            found += 1;
        }
        else if(open.exists(num0) == false){
            not_found += 1;
        }
    }

    cout << "FOUND = " << found << endl;
    cout << "NOT FOUND = " << not_found << endl;

}
template<typename record>
void test_hash_table_interactive(chained_hash<record>& open, string name){
    srand(time(NULL));

    cout << "**********************************************************" << endl;
    cout << "          INTERACTIVE HASH TEST : " << name << "           " << endl;
    cout << "**********************************************************" << endl;

    char ans = '\0';
    do{
        cout << "[S]ize() [I]nsert [R]andom [D]elete [F]ind value "
             <<   "[?]exists?   e[X]it  :" << endl;
        cin >> ans;
        if(ans == 's' || ans == 'S'){
            cout << "SIZE = " << open.get_size() << endl;
        }
        else if(ans == 'I' || ans == 'i'){
            int num, num2;
            cin >> num >> num2;
            record sub(num,num2);
            if(open.insert(sub) == true){
                cout << "<< " << num << ":" << num2 << " has been INSERTED." << endl;
            }
            else if(open.insert(sub) == false){
                cout << num << ":" << num2 << " was NOT inserted." << endl;
            }
            open.print();
            cout << "SIZE = " << open.get_size() << endl;
        }
        else if(ans == 'R' || ans == 'r'){
            int num, num2;
            num = rand()%10000;
            num2 = rand()%10000;
            record sub(num,num2);
            if(open.insert(sub) == true){
                cout << "<< " << num << ":" << num2 << " has been INSERTED." << endl;
            }
            else if(open.insert(sub) == false){
                cout << num << ":" << num2 << " was NOT inserted." << endl;
            }
            open.print();
            cout << "SIZE = " << open.get_size() << endl;
        }
        else if(ans == 'd' || ans == 'D'){
            int num;
            cin >> num;
            if(open.remove(num) == true){
                cout << "<< " << num << " was REMOVED." << endl;
            }
            else if(open.remove(num) == false){
                cout << "num was NOT found" << endl;
            }
            open.print();
            cout << "SIZE = " << open.get_size() << endl;
        }
        else if(ans == 'f' || ans == 'F'){
            record sub;
            int num;
            cin >> num;
            open.found(num,sub);
            if(open.found(num,sub) == true){
                cout << sub << " was FOUND" << endl;
            }
            else if(open.found(num,sub) == false){
                cout << "<< " << num << " was NOT found" << endl;
            }
        }
        else if(ans == '?'){
            int num;
            cin >> num;
            if(open.exists(num) == true){
                cout << "<< " << num << " EXISTS" << endl;
            }
            else if(open.exists(num) == false){
                cout << "<< " << num << " does NOT exist" << endl;
            }
        }
        else if(ans == 'X' || ans == 'x'){
            cout << "DONE" << endl;
            exit(1);
        }
    }while(ans != 'X' || ans != 'x');
}
template<typename record>
void test_hash_table_interactive(open_hash<record>& open, string name){
    srand(time(NULL));

    cout << "**********************************************************" << endl;
    cout << "          INTERACTIVE HASH TEST : " << name << "           " << endl;
    cout << "**********************************************************" << endl;

    char ans = '\0';
    do{
        cout << "[S]ize() [I]nsert [R]andom [D]elete [F]ind value "
             <<   "[?]exists?   e[X]it  :" << endl;
        cin >> ans;
        if(ans == 's' || ans == 'S'){
            cout << "SIZE = " << open.get_size() << endl;
        }
        else if(ans == 'I' || ans == 'i'){
            int num, num2;
            cin >> num >> num2;
            record sub(num,num2);
            if(open.insert(sub) == true){
                cout << "<< " << num << ":" << num2 << " has been INSERTED." << endl;
                cout << open;
                cout << "collisions = " << open.get_collisons() << endl;
            }
            else if(open.insert(sub) == false){
                cout << num << ":" << num2 << " was NOT inserted." << endl;
            }
        }
        else if(ans == 'R' || ans == 'r'){
            int num, num2;
            num = rand()%10000;
            num2 = rand()%10000;
            record sub(num,num2);
            if(open.insert(sub) == true){
                cout << num << ":" << num2 << " has been INSERTED." << endl;
                cout << open;
                cout << "collisions = " << open.get_collisons() << endl;
            }
            else if(open.insert(sub) == false){
                cout << num << ":" << num2 << " was NOT inserted." << endl;
            }
        }
        else if(ans == 'd' || ans == 'D'){
            int num;
            cin >> num;
            if(open.remove(num) == true){
                cout << "<< " << num << " was REMOVED." << endl;
                cout << open;
            }
            else if(open.remove(num) == false){
                cout << "num was NOT found" << endl;
            }
        }
        else if(ans == 'f' || ans == 'F'){
            bool maybe = false;
            int num;
            int index;
            cin >> num;
            open.found(num,maybe,index);
            if(maybe == true){
                cout << open.get_data(index) << " was FOUND" << endl;
            }
            else if(maybe == false){
                cout << "<< " << num << " NOT found" << endl;
            }
        }
        else if(ans == '?'){
            int num;
            cin >> num;
            if(open.exists(num) == true){
                cout << "<< " << num << " EXISTS" << endl;
            }
            else if(open.exists(num) == false){
                cout << "<< " << num << " does NOT exist" << endl;
            }
        }
        else if(ans == 'X' || ans == 'x'){
            cout << "DONE" << endl;
            exit(1);
        }
    }while(ans != 'X' || ans != 'x');
}
void test_hash_table_interactive(double_hash<record>& open, string name){
    srand(time(NULL));

    cout << "**********************************************************" << endl;
    cout << "          INTERACTIVE HASH TEST : " << name << "           " << endl;
    cout << "**********************************************************" << endl;

    char ans = '\0';
    do{
        cout << "[S]ize() [I]nsert [R]andom [D]elete [F]ind value "
             <<   "[?]exists?   e[X]it  :" << endl;
        cin >> ans;
        if(ans == 's' || ans == 'S'){
            cout << "SIZE = " << open.get_size() << endl;
        }
        else if(ans == 'I' || ans == 'i'){
            int num, num2;
            cin >> num >> num2;
            record sub(num,num2);
            if(open.insert(sub) == true){
                cout << "<< " << num << ":" << num2 << " has been INSERTED." << endl;
                cout << open;
                cout << "collisions = " << open.get_collisons() << endl;
            }
            else if(open.insert(sub) == false){
                cout << num << ":" << num2 << " was NOT inserted." << endl;
            }
        }
        else if(ans == 'R' || ans == 'r'){
            int num, num2;
            num = rand()%10000;
            num2 = rand()%10000;
            record sub(num,num2);
            if(open.insert(sub) == true){
                cout << num << ":" << num2 << " has been INSERTED." << endl;
                cout << open;
                cout << "collisions = " << open.get_collisons() << endl;
            }
            else if(open.insert(sub) == false){
                cout << num << ":" << num2 << " was NOT inserted." << endl;
            }
        }
        else if(ans == 'd' || ans == 'D'){
            int num;
            cin >> num;
            if(open.remove(num) == true){
                cout << "<< " << num << " was REMOVED." << endl;
                cout << open;
            }
            else if(open.remove(num) == false){
                cout << "num was NOT found" << endl;
            }
        }
        else if(ans == 'f' || ans == 'F'){
            bool maybe = false;
            int num;
            int index;
            cin >> num;
            open.found(num,maybe,index);
            if(maybe == true){
                cout << open.get_data(index) << " was FOUND" << endl;
            }
            else if(maybe == false){
                cout << "<< " << num << " NOT found" << endl;
            }
        }
        else if(ans == '?'){
            int num;
            cin >> num;
            if(open.exists(num) == true){
                cout << "<< " << num << " EXISTS" << endl;
            }
            else if(open.exists(num) == false){
                cout << "<< " << num << " does NOT exist" << endl;
            }
        }
        else if(ans == 'X' || ans == 'x'){
            cout << "DONE" << endl;
            exit(1);
        }
    }while(ans != 'X' || ans != 'x');
}
void collision_test(int iterations){
    cout << "**********************************************************" << endl;
    cout << "          RANDOM OPEN_HASH VS DOULBE_HASH TEST " << endl;
    cout << "**********************************************************" << endl;

    open_hash<record> open;
    double_hash<record> dub;



    record sub[iterations];
    for(int i = 0; i < iterations; ++i){
        int num0,  num2;
        num0 = rand()%10000;
        num2 = rand()%10000;
        record kk(num0,num2);
        sub[i] = kk;
    }




    Timer test;
    test.start();
    for(int i = 0; i < iterations; ++i){
        record pol(sub[i]._key,sub[i]._value);

        open.insert(pol);
    }
    test.stop();

    //cout << "duratoin = " << test.duration() << endl;

    Timer test2;
    test2.start();
    for(int i = 0; i < iterations; ++i){
        record pol(sub[i]._key,sub[i]._value);
        dub.insert(pol);
    }\
    test2.stop();

    //cout << "duratoin = " << test2.duration() << endl;

    cout << "OPEN_HASH COLLISIONS = " << open.get_collisons() << endl;
    cout << "DOUBLE_HASH COLLISIONS = " << dub.get_collisons() << endl;


}




//******************CHAINED HASH BELOW
template<typename record>
bool chained_hash<record>::remove(int num){
    if(exists(num) == false){
        return false;
    }
    int key;
    key = num % SZ;

    LinkedList *ptr, *head, *behind;
    ptr = front[key].connect;
    head = front[key].connect;
    behind = front[key].connect;

    if(ptr->data._key == num){

        behind = ptr->link;
        delete ptr;
        front[key].connect = behind;
        size -= 1;
        return true;
    }
    else if(ptr->data._key != num){
        ptr = ptr->link;
        if(ptr->data._key == num){
            behind->link = ptr->link;
            behind = ptr;
            delete ptr;
            front[key].connect = behind;
            size -= 1;
            return true;
        }
        ptr = ptr->link;
        behind = behind->link;
        while(ptr->link != NULL){
            if(ptr->data._key == num){
                behind->link = ptr->link;
                behind = ptr;
                delete ptr;
                front[key].connect = behind;
                size -= 1;
                return true;
            }
            ptr = ptr->link;
            behind = behind->link;
        }
    }
}
template<typename record>
int chained_hash<record>::get_size(){
    return size;
}
template<typename record>
bool chained_hash<record>::found(int num, record& sub){
    int key;
    key = num % SZ;
    if(NULL == front[key].connect){
        return false;
    }
    else if(front[key].connect != NULL){
        if(front[key].connect->data._key == num){
            sub._key = front[key].connect->data._key;
            sub._value = front[key].connect->data._value;
            return true;
        }
        LinkedList *in;
        in = front[key].connect->link;
        if(in == NULL){
            return false;
        }
        while(in != NULL){
            if(in->data._key > 0){
                if(in->data._key == num){
                    sub._key = in->data._key;
                    sub._value = in->data._value;
                    return true;
                }
            }
            in = in->link;
            if(in == NULL){
                return false;
            }
        }
    }
}
template<typename record>
bool chained_hash<record>::exists(int num){
    int key;
    key = num % SZ;
    if(NULL == front[key].connect){
        return false;
    }
    else if(front[key].connect != NULL){
        if(front[key].connect->data._key == num){
            return true;
        }
        LinkedList *ptr;
        ptr = front[key].connect->link;
        if(ptr == NULL){
            return false;
        }
        while(ptr != NULL){
//            if(-1 == ptr->data._key){
//                return false;
//            }
            if(ptr->data._key > 0){
                if(ptr->data._key == num){
                    ptr->data._value = num;
                    return true;
                }
            }
            ptr = ptr->link;
            if(ptr == NULL){
                return false;
            }
        }
    }
}
template<typename record>
bool chained_hash<record>::exists_Change(const record& entry){
    int key;
    key = entry._key % SZ;
    if(NULL == front[key].connect){
        return false;
    }
    if(front[key].connect != NULL){
        if(front[key].connect->data._key == entry._key){
            front[key].connect->data._value = entry._value;
            return true;
        }
        LinkedList *ptr;
        ptr = front[key].connect->link;
        if(ptr == NULL){
            return false;
        }
        while(ptr != NULL){
            if(NULL == ptr->data._key){
                return false;
            }
            else if(ptr != NULL){
                if(ptr->data._key == entry._key){
                    ptr->data._value = entry._value;
                    return true;
                }
            }
            ptr = ptr->link;
            if(ptr == NULL){
                return false;
            }
        }
    }
}
template<typename record>
bool chained_hash<record>::insert(const record& entry){
    if(exists_Change(entry) == true){
        return true;
    }

    int key;
    key = entry._key % SZ;

    if(NULL == front[key].connect){
        front[key].connect = new LinkedList;
        front[key].connect->data._key = entry._key;
        front[key].connect->data._value = entry._value;
        size += 1;
        return true;
    }
    if(front[key].connect != NULL){
        if(front[key].connect->link == NULL){
            front[key].connect->link = new LinkedList;
            front[key].connect->link->data._key = entry._key;
            front[key].connect->link->data._value = entry._value;
            size += 1;
            return true;
        }
        else if(front[key].connect->link != NULL){
            LinkedList *ptr;
            ptr = front[key].connect->link;
            while(front[key].connect->link != NULL){

                if(ptr->link == NULL){
                    ptr->link = new LinkedList;
                    ptr->link->data._key = entry._key;
                    ptr->link->data._value = entry._value;
                    size += 1;
                    return true;
                }
                ptr = ptr->link;
            }
        }
    }
}
template<typename record>
void chained_hash<record>::print(ostream& outs) const{
    for(int i = 0; i < SZ; ++i){
        outs << "data["<<i<<"] = ";
        if(front[i].connect == NULL){

        }
        else if(front[i].connect != NULL){
            cout << "[" << front[i].connect->data << "]";
            LinkedList *ptr;
            ptr = front[i].connect->link;
            while(ptr != NULL){
                outs << "-> ";
                outs << "[" << ptr->data << "]";
                ptr = ptr->link;
            }
        }
        outs << "  |||" << endl;
    }
}
template<typename record>
chained_hash<record>::chained_hash(){

    for(int i = 0; i < SZ; ++i){
        front[i].connect = NULL;
    }
    size = 0;
}
LinkedList::LinkedList(){
    link = NULL;
}
store::store(){
    connect = NULL;
}
template<typename h>
ostream& operator <<(ostream& outs, const chained_hash<h>& print_me){
    print_me.print(outs);
    return outs;
}
template<typename record>
store chained_hash<record>::get_data(int index){
    return front[index];
}





//           OPEN_HASH BELOW     ********************
template<typename record>
void open_hash<record>::insert_again(const record &entry, int key, bool& maybe){
    if(key == SZ){
        insert_again(entry,0, maybe);
        return;
    }
    collisons += 1;
    if(-1 == data[key]._key || -2 == data[key]._key){
        data[key] = entry;
        size += 1;
        maybe = true;
        return;
    }
    if(data[key]._key > 0){
       insert_again(entry,key+1, maybe);
    }
}
template<typename record>
bool open_hash<record>::insert(const record& entry){

    if(size == SZ){
        return false;
    }
    if(check_Dup(entry) == true){
        return true;
    }

    int key;
    key = entry._key % SZ;

    if(-1 == data[key]._key || -2 == data[key]._key){
        data[key] = entry;
        size += 1;
        return true;
    }
    if(data[key]._key > 0){
       bool maybe = false;
       insert_again(entry,key+1, maybe);
       if(maybe == true){
           return true;
       }
       else if(maybe == false){
           return false;
       }
    }
}
template<typename record>
void open_hash<record>::exists_Again(int entry, bool& maybe, int& index){
    if(index == SZ){
        index = 0;
        exists_Again(entry, maybe, index);
        return;
    }
    if(index == entry % SZ){
        maybe = false;
        return;
    }
    if(data[index]._key == entry){
        maybe = true;
        return;
    }
    else if(-1 == data[index]._key){
        maybe = false;
        return;
    }
    else if(-2 == data[index]._key || data[index]._key > 0){
        index += 1;
        exists_Again(entry,maybe, index);
    }
}
template<typename record>
bool open_hash<record>::exists(int entry){
    int key;
    key = entry % SZ;
    if(data[key]._key == entry){
        return true;
    }
    else if(-1 == data[key]._key){
        return false;
    }
    else if(-2 == data[key]._key || data[key]._key > 0){
        int index;
        index = key + 1;
        bool maybe = false;
        exists_Again(entry,maybe, index);
        if(maybe == true){
            return true;
        }
        else if(maybe == false){
            return false;
        }
    }
}
template<typename record>
int open_hash<record>::get_size() const{
    return size;
}
template<typename h>
ostream& operator <<(ostream& outs, const open_hash<h>& print_me){
    print_me.print(outs);
    return outs;
}
template<typename record>
void open_hash<record>::found_Again(int entry, bool &maybe, int& index){
    if(index == SZ){
        index = 0;
        found_Again(entry, maybe, index);
        return;
    }
    if(index == entry % SZ){
        maybe = false;
        return;
    }
    if(data[index]._key == entry){
        maybe = true;
        return;
    }
    else if(-1 == data[index]._key){
        maybe = false;
        return;
    }
    else if(-2 == data[index]._key || data[index]._key > 0){
        index += 1;
        found_Again(entry,maybe, index);
    }
}
template<typename record>
record open_hash<record>::get_data(int index){
    return data[index];
}
template<typename record>
void open_hash<record>::found(int entry, bool& maybe, int& index){
    int key;
    key = entry % SZ;
    if(data[key]._key == entry){
        maybe = true;
        index = key;
        return;
    }
    else if(-1 == data[key]._key){
        maybe = false;
        return;
    }
    else if(-2 == data[key]._key || data[key]._key > 0){
        index = key + 1;
        found_Again(entry,maybe, index);
    }
}
template<typename record>
void open_hash<record>::remove_Again(int entry, int key, bool &maybe){
    if(key == SZ){
        remove_Again(entry,0, maybe);
        return;
    }
    if(key == entry % SZ){
        maybe = false;
        return;
    }
    if(data[key]._key == entry){
        data[key]._key = -2;
        data[key]._value = 0;
        size -= 1;
        maybe =  true;
        return;
    }

    if(-1 == data[key]._key){
        maybe = false;
        return;
    }

    else if(-2 == data[key]._key || data[key]._key > 0){
        remove_Again(entry,key + 1,maybe);
        return;
    }

}
template<typename record>
bool open_hash<record>::remove(int entry){
    int key;
    key = entry % SZ;
    if(data[key]._key == entry){
        data[key]._key = -2;
        data[key]._value = 0;
        size -= 1;
        return true;
    }
    else if(-1 == data[key]._key){
        return false;
    }
    else if(-2 == data[key]._key || data[key]._key > 0){
        bool maybe = false;
        remove_Again(entry,key + 1,maybe);
        if(maybe == true){
            return true;
        }
        else if(maybe == false){
            return false;
        }
    }
}
template<typename record>
void open_hash<record>::check_Dup_Again(const record& entry, int key, bool& maybe){
    if(key == SZ){
        check_Dup_Again(entry,0, maybe);
        return;
    }
    if(key == entry._key % SZ){
        maybe = false;
        return;
    }
    if(data[key]._key == entry._key){
        data[key]._value = entry._value;
        maybe = true;
        return;
    }
    else if(-2 == data[key]._key){
        check_Dup_Again(entry,key + 1,maybe);
        return;
    }
    else if(-1 == data[key]._key){
        maybe = false;
        return;
    }
}
template<typename record>
bool open_hash<record>::check_Dup(const record &entry){
    int key;
    key = entry._key % SZ;
    if(data[key]._key == entry._key){
        data[key]._value = entry._value;
        return true;
    }
    else if(-2 == data[key]._key || data[key]._key > 0){
        bool maybe = false;
        check_Dup_Again(entry,key + 1,maybe);
        if(maybe == true){
            return true;
        }
        else if(maybe == false){
            return false;
        }
    }
    else if(-1 == data[key]._key){
        return false;
    }
}
template<typename record>
void open_hash<record>::print(ostream& outs) const{
    for(int i = 0; i < SZ; ++i){
        outs << "data["<<i<<"] = ";
        if(-1 == data[i]._key){
            //cout << endl;
        }
        else if(data[i]._key > 0){
            cout << data[i];
            int key;
            key = data[i]._key % SZ;
            outs << "(" << key << ")";
            if(key != i){
                cout << "*";
            }
        }
        else if(-2 == data[i]._key){
            outs << " - - - - - - - - -";
        }
        outs << endl;
    }
}
template<typename record>
open_hash<record>::open_hash(){
    for(int i = 0; i < SZ; ++i){
        data[i]._key = -1;
    }
    size = 0;
    collisons = 0;
}
ostream& operator <<(ostream& outs, const record& print_me){
    outs << print_me._key << " : " << print_me._value;

}
record::record(int k, double v){
    _key = k;
    _value = v;
}
template<typename record>
int open_hash<record>::get_collisons(){
    return collisons;
}



//******************DOUBLE_HASH BELOW
template<typename record>
void double_hash<record>::remove_Again(int entry, int key, bool &maybe, int hash2){
    if(key == entry % SZ){
        maybe = false;
        return;
    }
    if(data[key]._key == entry){
        data[key]._key = -2;
        data[key]._value = 0;
        size -= 1;
        maybe =  true;
        return;
    }

    if(-1 == data[key]._key){
        maybe = false;
        return;
    }

    else if(-2 == data[key]._key || data[key]._key > 0){
        //hash2 = 1 + (key % (SZ - 2));
        key = (key + hash2) % SZ;
        if(key >= SZ){
            int dif;
            dif = key - SZ;
            key = dif;
        }
        remove_Again(entry,key,maybe, hash2);
        return;
    }

}
template<typename record>
bool double_hash<record>::remove(int entry){
    int key;
    key = entry % SZ;
    if(data[key]._key == entry){
        data[key]._key = -2;
        data[key]._value = 0;
        size -= 1;
        return true;
    }
    else if(-1 == data[key]._key){
        return false;
    }
    else if(-2 == data[key]._key || data[key]._key > 0){
        bool maybe = false;
        int hash2;
        hash2 = 1 + (key % (SZ - 2));
        key = (key + hash2) % SZ;
        if(key >= SZ){
            int dif;
            dif = key - SZ;
            key = dif;
        }
        remove_Again(entry,key,maybe, hash2);
        if(maybe == true){
            return true;
        }
        else if(maybe == false){
            return false;
        }
    }
}
template<typename record>
void double_hash<record>::found_Again(int entry, bool &maybe, int& index, int hash2){

    if(index == entry % SZ){
        maybe = false;
        return;
    }
    if(data[index]._key == entry){
        maybe = true;
        return;
    }
    else if(-1 == data[index]._key){
        maybe = false;
        return;
    }
    else if(-2 == data[index]._key || data[index]._key > 0){
        //hash2 = 1 + (index % (SZ - 2));
        index = (index + hash2) % SZ;
        if(index >= SZ){
            int dif;
            dif = index - SZ;
            index = dif;
        }
        found_Again(entry,maybe, index, hash2);
    }
}
template<typename record>
void double_hash<record>::found(int entry, bool& maybe, int& index){
    int key;
    key = entry % SZ;
    if(data[key]._key == entry){
        maybe = true;
        index = key;
        return;
    }
    else if(-1 == data[key]._key){
        maybe = false;
        return;
    }
    else if(-2 == data[key]._key || data[key]._key > 0){
        int hash2;
        hash2 = 1 + (key % (SZ - 2));
        key = (key + hash2) % SZ;
        index = key;
        if(key >= SZ){
            int dif;
            dif = key - SZ;
            key = dif;
            index = key;
        }
        found_Again(entry,maybe, index, hash2);
    }
}
template<typename record>
void double_hash<record>::exists_Again(int entry, bool& maybe, int& index, int hash2){

    if(index == entry % SZ){
        maybe = false;
        return;
    }
    if(data[index]._key == entry){
        maybe = true;
        return;
    }
    else if(-1 == data[index]._key){
        maybe = false;
        return;
    }
    else if(-2 == data[index]._key || data[index]._key > 0){
        //hash2 = 1 + (index % (SZ - 2));
        index = (index + hash2) % SZ;
        if(index >= SZ){
            int dif;
            dif = index - SZ;
            index = dif;
        }
        exists_Again(entry,maybe, index, hash2);
    }
}
template<typename record>
bool double_hash<record>::exists(int entry){
    int key;
    key = entry % SZ;
    if(data[key]._key == entry){
        return true;
    }
    else if(-1 == data[key]._key){
        return false;
    }
    else if(-2 == data[key]._key || data[key]._key > 0){
        bool maybe = false;
        int hash2;
        hash2 = 1 + (key % (SZ - 2));
        key = (key + hash2) % SZ;
        if(key >= SZ){
            int dif;
            dif = key - SZ;
            key = dif;
        }
        exists_Again(entry,maybe, key, hash2);
        if(maybe == true){
            return true;
        }
        else if(maybe == false){
            return false;
        }
    }
}
template<typename record>
void double_hash<record>::check_Dup_Again(const record& entry, int key, bool& maybe, int hash2){

    if(key == entry._key % SZ){
        maybe = false;
        return;
    }
    if(data[key]._key == entry._key){
        data[key]._value = entry._value;
        maybe = true;
        return;
    }
    else if(-2 == data[key]._key || data[key]._key > 0){
        key = (key + hash2) % SZ;
        if(key >= SZ){
            int dif;
            dif = key - SZ;
            key = dif;
        }
        check_Dup_Again(entry,key,maybe, hash2);
        return;
    }
    else if(-1 == data[key]._key){
        maybe = false;
        return;
    }
}
template<typename record>
bool double_hash<record>::check_Dup(const record &entry){
    int key;
    key = entry._key % SZ;
    if(data[key]._key == entry._key){
        data[key]._value = entry._value;
        return true;
    }
    else if(-2 == data[key]._key || data[key]._key > 0){
        bool maybe = false;
        int hash2;
        hash2 = 1 + (key % (SZ - 2));
        key = (key + hash2) % SZ;
        if(key >= SZ){
            int dif;
            dif = key - SZ;
            key = dif;
        }
        check_Dup_Again(entry,key,maybe, hash2);
        if(maybe == true){
            return true;
        }
        else if(maybe == false){
            return false;
        }
    }
    else if(-1 == data[key]._key){
        return false;
    }
}
template<typename record>
void double_hash<record>::insert_again(const record &entry, int key, bool& maybe, int hash2){
    if(key == entry._key % SZ){
        maybe = false;
        return;
    }
    collisons += 1;
    if(-1 == data[key]._key || -2 == data[key]._key){
        data[key] = entry;
        size += 1;
        maybe = true;
        return;
    }
    if(data[key]._key > 0){
        //hash2 = 1 + (key % (SZ - 2));
        key = (key + hash2) % SZ;
        if(key >= SZ){
            int dif;
            dif = key - SZ;
            key = dif;
        }
       insert_again(entry,key, maybe, hash2);
    }
}
template<typename record>
bool double_hash<record>::insert(const record& entry){

    if(size == SZ){
        return false;
    }
    if(check_Dup(entry) == true){
        return true;
    }

    int key;
    key = entry._key % SZ;

    if(-1 == data[key]._key || -2 == data[key]._key){
        data[key] = entry;
        size += 1;
        return true;
    }
    if(data[key]._key > 0){
       bool maybe = false;
       int hash2;
       hash2 = 1 + (key % (SZ - 2));
       key = (key + hash2) % SZ;
       if(key >= SZ){
           int dif;
           dif = key - SZ;
           key = dif;
       }
       insert_again(entry,key, maybe,hash2);
       if(maybe == true){
           return true;
       }
       else if(maybe == false){
           return false;
       }
    }
}
template<typename record>
int double_hash<record>::get_size() const{
    return size;
}
template<typename h>
ostream& operator <<(ostream& outs, const double_hash<h>& print_me){
    print_me.print(outs);
    return outs;
}
template<typename record>
void double_hash<record>::print(ostream& outs) const{
    for(int i = 0; i < SZ; ++i){
        outs << "data["<<i<<"] = ";
        if(-1 == data[i]._key){
            //cout << endl;
        }
        else if(data[i]._key > 0){
            cout << data[i];
            int key;
            key = data[i]._key % SZ;
            outs << "(" << key << ")";
            if(key != i){
                cout << "*";
            }
        }
        else if(-2 == data[i]._key){
            outs << " - - - - - - - - -";
        }
        outs << endl;
    }
}
template<typename record>
double_hash<record>::double_hash(){
    for(int i = 0; i < SZ; ++i){
        data[i]._key = -1;
    }
    size = 0;
    collisons = 0;
}
template<typename record>
int double_hash<record>::get_collisons(){
    return collisons;
}
template<typename record>
record double_hash<record>::get_data(int index){
    return data[index];
}
