#include <iostream>
#include <vector>
#include <string>
#include <cmath>

struct BNode{
    std::string val;
    BNode* left;
    BNode* right;
};
 
BNode* bt_node(std::string e,BNode* l,BNode* r){
    BNode* tmp = new BNode;
    tmp -> val = e;
    tmp -> left = l;
    tmp -> right = r;
    return tmp;
}
//finds next combination
void next_order(std::vector<int>& tmp,int size,int i){
    int y = 0,factor=tgamma(size);
    if(size == 0){
        return;
    }
    for(int j=0;j<=(i/factor)+y;j++){
        if(tmp[j]!=-1){
            y++;
        }
    }
    tmp[(i/factor)+y]=size-1;        
    i= i%factor;
    size--;
    next_order(tmp,size,i);
}
//finds every combination of numbers inputed
std::vector<std::vector<int>> all_possible_orders(int size){
    std::vector<std::vector<int>> out;
    std::vector<int> tmp,neg = {};
    for(int i = 0;i<size;i++){
        neg.push_back(-1);
    }
    for(int i =0;i<tgamma(size+1);i++){
        tmp = neg;
        next_order(tmp,size,i);
        out.push_back(tmp);
    }
    return out;
}

std::vector<std::string> in_order(std::vector<std::string> r,std::vector<int> order){
    std::vector<std::string> out=r;
    for(int i=0;i<r.size();i++){
        for(int j=0;j<r[0].size();j++){
            out[i][j]=r[i][order[j]];
        }
    }
    return out;
}
// predicts the number of nodes if the tree is built
int predicted_number_of_nodes(std::vector<std::string> r){
    std::vector<std::string> same_branch, different_branch;
    int y=1;
    if(r[0].size()==0){
        return 1;
    }
    if(r[0][0]=='x'){
        for(int i=0;i<r.size();i++){
            r[i] = r[i].substr(1);
        }
        return predicted_number_of_nodes(r);
    }
    while(y < r.size()){
        if(r[0][0]==r[y][0]){
            same_branch.push_back(r[y].substr(1));
        }else{
            different_branch.push_back(r[y].substr(1));
        }
        y++;
    }
    same_branch.push_back(r[0].substr(1));
    if(different_branch.size()==0){
        return 2 + predicted_number_of_nodes(same_branch);
    }
    return 1 + predicted_number_of_nodes(same_branch) + predicted_number_of_nodes(different_branch);
}
//reduces the inputs into rows with dont cares
std::vector<std::string> shorthand(std::vector<std::string> r){
    std::vector<std::string> tmp;
    for(int y=r[0].size()-1; y>=0;y--){
        int i=0;
        while(i<r.size()){
            int j = i+1,check = 1;
            while((j < r.size())&&(check!=-1)){
                int x =0;
                check=-1;
                while((x != r[0].size())&&(check == -1)){
                    if((r[i][x]!=r[j][x])&&(x!=y)){
                        check = 0;
                    }
                    x++;
                }
                if(check == -1){
                    r[i][y]='x';
                    r[j]="";
                }
                j++;
                if(r[j]==""){
                    j++;
                }
            }
            if(r[i]!=""){
                tmp.push_back(r[i]);
            }
            i++;
            if(r[i]==""){
                i++;
            }
        }
        r = tmp;
        tmp.clear();
    }
    return r;
}
//Bases the order of the varible the least dont cares to the most
std::vector<int> order(std::vector<std::string> in){
    std::vector <int> rank;
    std::vector<int> out;
    for(int i=0;i<in[0].size();i++){
        int counter = 0;
        for(int j=0;j<in.size();j++){
            if(in[j][i]=='x'){
                counter++;
            }
        }
        rank.push_back(counter);
    }
    for(int a = 0; a < in[0].size();a++){
        int min = -1, track;
        for(int k=0;k<rank.size();k++){
            if(((rank[k] < min)&&(rank[k]!=-1))||(min==-1)){
                min = rank[k];
                track = k;
            }
        }
        rank[track]=-1;
        out.push_back(track);
    }
    out.push_back(-1);
    return out;
}
//Checks if an order and compressed input is valid
bool checker(std::vector<std::string> r,std::vector <int> orders){
    int s=0,che = 0;
    while((s<r.size())&&(che==0)){
        int x=s+1;
        while((x<r.size())&&(che==0)){
            int y =0,same = 0;
            while((y<r[0].size())&&(same==0)){
                same=1;
                if(r[s][orders[y]]==r[x][orders[y]]){
                    same=0;
                }
                else if((r[s][orders[y]]=='x')||(r[x][orders[y]]=='x')){
                    che++;
                }
                y++;
            }
            x++;
        }
        s++;
    }
    if(che == 1){
        return 0;
    }
    return 1;
}
//Finds the problem and expands it to fix it
std::vector<std::string> replace(std::vector<std::string> r,std::vector <int> orders){
    std::string tmp;
    int saves,savey;
    int s=0,che = 0;
    while((s<r.size())&&(che==0)){
        int x=s+1;
        while((x<r.size())&&(che==0)){
            int y =0,same = 0;
            while((y<r[0].size())&&(same==0)){
                same=1;
                if(r[s][orders[y]]==r[x][orders[y]]){
                    same=0;
                }
                else if(r[s][orders[y]]=='x'){
                    che++;
                    saves =s;
                    savey =y;
                }else if(r[x][orders[y]]=='x'){
                    che++;
                    saves =x;
                    savey =y;
                }
                y++;
            }
            x++;
        }
        s++;
    }
    if(che == 1){
        tmp = r[saves];
        tmp[orders[savey]]='1';
        r[saves][orders[savey]]='0';
        r.push_back(tmp);
        return replace(r,order(r));
    }
    return r;
}
//Works through all orders and finds the valid one with the least nodes
std::vector<int> min_order(std::vector<std::string> r){
    std::vector<std::vector<int>> pos_order = all_possible_orders(r[0].size());
    int size = -1,tmp;
    std::vector<int> min;
    for(int i=0;i<pos_order.size();i++){
        if(checker(r,pos_order[i])){
            tmp = predicted_number_of_nodes(in_order(r,pos_order[i]));
            if((tmp<size)||(size==-1)){
                size=tmp;
                min = pos_order[i];  
            }
        }
    }
    min.push_back(-1);
    return min;
}
//builds a branch of the exsisting tree
BNode* hit_0(std::string const direction,std::vector<int> const ord,int counter){
    std::string name;
    char x01 = direction[ord[counter]];
    name = "x" + std::to_string(ord[counter] + 1);
    if(x01 == 'x'){
        return hit_0(direction,ord,counter+1);
    }
    if(ord[counter]==-1){
        return bt_node("1",NULL,NULL);
    }
    if(x01 == '1'){
        return bt_node(name,bt_node("0",NULL,NULL),hit_0(direction,ord,counter+1));
    }
    return bt_node(name,hit_0(direction,ord,counter+1),bt_node("0",NULL,NULL));
}
//finds the zero on the path to place the one
BNode* Build_tree(std::string const direction,std::vector<int> const ord,BNode* t,int counter){
    char x01 = direction[ord[counter]];
    if(t -> val != "0"){
        if(x01=='0'){
        t -> left = Build_tree(direction,ord,t->left,counter+1);
        return t;
        }
        if(x01=='1'){
            t -> right = Build_tree(direction,ord,t->right,counter+1);
            return t;
        }
        return Build_tree(direction,ord,t,counter+1);
    }
    return hit_0(direction,ord,counter);
}

BNode* build_bt(const std::vector<std::string>& fvalues){
    BNode* t;
    t = bt_node("0",NULL,NULL);
    if (fvalues.size() > 0){
        std::vector<std::string> r = shorthand(fvalues);
        std::vector<int> the_order = min_order(r);
        if(the_order[0]==-1){
            do{
            the_order = order(r);
            r=replace(r,the_order);
            }while(checker(r,the_order)==0);
        }
        for(int i=0;i<r.size();i++){
            t=Build_tree(r[i],the_order,t,0);
        }
    }
    return t;
}

int label_to_idx(const std::string& label){
 
    std::string out;
 
    for(int i = 1; i < label.size(); i++){
        out.push_back(label[i]);
    }
 
    return std::stoi(out) - 1;
}

void destroy_tree(BNode* T){
  if(T != NULL){
    destroy_tree(T->left);
    destroy_tree(T->right);
    delete T;
  }
}
 
std::string eval_bt(BNode* bt, const std::string& input){
 
    if( (bt->left == NULL) && (bt->right == NULL) ){
        return bt->val;
    }
    else{
        int idx = label_to_idx(bt->val);
        std::string input_idx;
        input_idx.push_back(input[idx]);
 
        if(input_idx == "0"){
            return eval_bt(bt->left, input); 
        }
        else{
            return eval_bt(bt->right, input); 
        }
    }
}
 
int n_nodes_bt(BNode* t){
    if(t == NULL){
        return 0;
    }
    else{
        return 1 + n_nodes_bt(t->left) + n_nodes_bt(t->right);
    }
}
 
class BoolTree{
public:
 
    BoolTree(const std::vector<std::string>& fvalues){
       t = build_bt(fvalues);
    }
 
    std::string eval(const std::string& s){
        return eval_bt(t, s);
    }
 
    int n_nodes(){
        return n_nodes_bt(t);
    }
 
    ~BoolTree(){
        destroy_tree(t);
    }
 
private:
    BNode* t;
};
