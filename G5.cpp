#include <iostream>
#include <vector>
#include <string>

struct BNode{
    std::string val;
    BNode* left;
    BNode* right;
};
 
struct oneandzero{
    std::vector<std::string> one;
    std::vector<std::string> zero;
};

BNode* bt_node(std::string e,BNode* l,BNode* r){
    BNode* tmp = new BNode;
    tmp -> val = e;
    tmp -> left = l;
    tmp -> right = r;
    return tmp;
}

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

//returns -1 if there are only x's or s's left and -2 if nothing is left
int order(std::vector<std::string> in){
    int out = -1,min = -1,max;
    if(in.size()==0){
        return -2;
    }
    for(int i=0;i<in[0].size();i++){
        int counter = 0,n1=0,n0=0;
        if(in[0][i]!='s'){
            for(int j=0;j<in.size();j++){
                if(in[j][i]=='x'){
                    counter++;
                }else if(in[j][i]=='1'){
                    n1++;
                }else{
                    n0++;
                }
            }
            if(n1 < n0){
                n1 = n0;
            }
            if((counter<min)||(min==-1)){
                min=counter;
                out = i;
                max = n1;
            }else if((counter == min)&&(n1>max)){
                min=counter;
                out = i;
                max = n1;
            }
        }
    }
    if(min==in.size()){
        return -1;
    }
    return out;
}

oneandzero Left_or_right(std::vector<std::string> r,int ord){
    oneandzero out;
    std::string tmp;
    for(int i=0;i<r.size();i++){
        tmp = r[i];
        tmp[ord] = 's';
        if(r[i][ord]=='1'){
            out.one.push_back(tmp);
        }else if(r[i][ord]=='0'){
            out.zero.push_back(tmp);
        }else{
            out.one.push_back(tmp);
            out.zero.push_back(tmp);
        }
    }
    return out;
}

BNode* Build_tree(std::vector<std::string> direction,int ord){
    std::string name;
    if(ord==-1){
        return bt_node("1",NULL,NULL);
    }
    if(ord==-2){
        return bt_node("0",NULL,NULL);
    }
    oneandzero next = Left_or_right(direction,ord);
    name = "x" + std::to_string(ord + 1);
    return bt_node(name,Build_tree(next.zero,order(next.zero)),Build_tree(next.one,order(next.one)));
}

// do not alter the following line that begins the function build_bt
BNode* build_bt(const std::vector<std::string>& fvalues){
    std::vector<std::string> r ;
    BNode* t = bt_node("0",NULL,NULL); 
    if(fvalues.size()!=0){
        r = shorthand(fvalues);
        t = Build_tree(r,order(r));
    }
    return t;
}

void destroy_tree(BNode* T){
    if(T != NULL){
        destroy_tree(T->left);
        destroy_tree(T->right);
        delete T;
    }
}

// do not alter the following function
// this function converts e.g. std::string "x3" to int 2
int label_to_idx(const std::string& label){
 
    std::string out;
 
    for(int i = 1; i < label.size(); i++){
        out.push_back(label[i]);
    }
 
    return std::stoi(out) - 1;
}
 
// do not alter the following function
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
 
// do not alter the following function
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
 
// the main provided below must work correctly
// with your implementation for the code above
// however you can change it as you wish for your own testing 
// and your changes won't be considered for the assessment
// (in your submission you can also have an empty main or no main at all)
 
int main(){
 
    std::vector<std::string> fv;
    std::string row;
 
    row = "11";
    fv.push_back(row);
 
    BoolTree ft1(fv);
    // as in the second assignment we give as input only the rows
    // of the truth table whose value is 1
    // (this is an example with the boolean "and" function)
 
    fv.clear();
 
    row = "010";
    fv.push_back(row);
    row = "011";
    fv.push_back(row);
    row = "110";
    fv.push_back(row);
    row = "111";
    fv.push_back(row);
 
    BoolTree ft2(fv);
    // this corresponds to the f(x1, x2, x3) example shown above
 
    std::cout << ft1.n_nodes() << std::endl;
    // we expect this to print 5
 
    std::cout << ft2.n_nodes() << std::endl;
    // if the algorithm is such that it builds the smallest possible corresponding tree
    // for the boolean function we are considering
    // then this will print 3 (see tree diagram in the example above)
 
    std::cout << ft1.eval("01") << std::endl;
    // this should print "0" 
 
    std::cout << ft1.eval("11") << std::endl;
    // this should print "1"
 
    std::cout << ft2.eval("001") << std::endl;
    // this should print "0"
 
    std::cout << ft2.eval("110") << std::endl;
    // this should print "1"
 
}