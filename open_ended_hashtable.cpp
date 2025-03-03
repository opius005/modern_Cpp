

// question -  https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqa01aN3hDc1FQdGkydnlGcF9uOHFXdW5IdFlUUXxBQ3Jtc0tuNER6Smh1N0VzNWVnMXR4Yzl3dHZESHRNY2pka3JHdFRJX2EtTkxYNDUzekprVTVZVGRLTDhtVFFfNFA1czIwQkM3bHd2VHpHN1VnVV9Dd3Y2bGluaUtWRzhNOFNNVnZDQzZtQS1SQTVNUXhjVHFMaw&q=https%3A%2F%2Fgithub.com%2Fzhxinyu%2Fcpp_interview&v=vGppE_ZlNWE


#include <iostream>
#include <limits>
#include <vector>

namespace ns
{
    template <typename key_type,typename value_type>
    class Open_Hash_table
    {
    private:
        struct Node
        {
            key_type key_;
            value_type value_;
            bool is_occupied_=false;
        };
        
        size_t size_;
        size_t capacity_;
        std::vector<Node> table_;

        size_t hash(const key_type &key) const
        {
            return std::hash<key_type>{}(key)%capacity_;
        }

        void expand()
        {
            if(capacity_>std::numeric_limits<size_t>::max()/(size_t)2)
            {
                std::cout<<"maximum capacity reached\n";
                return;
            }
            capacity_=capacity_*(size_t)2;
            std::vector<Node> old_table=table_;
            table_.clear();
            table_.resize(capacity_);
            size_=0;
            for(Node node:old_table)
            {
                if(node.is_occupied_)
                {
                    insert(node.key_,node.value_);
                }
            }
        }

    public:
        Open_Hash_table(size_t cap= 8): capacity_{cap},size_{0}
        {
            table_.resize(capacity_);
        }

        void insert(const key_type &key,const value_type &value) 
        {
            if(size_==(capacity_/(size_t)2))
            {
                expand();
            }
            size_t index=hash(key);
            while(table_[index].is_occupied_)
            {
                if(table_[index].key_==key)
                {
                    table_[index].value_=value;
                    return;
                }
                index=(index+(size_t)1)%capacity_;
            }
            table_[index].key_=key;
            table_[index].value_=value;
            table_[index].is_occupied_=true;
            size_++;
        }
        
        bool contains(const key_type &key) const
        {
            size_t index=hash(key);
            while(table_[index].is_occupied_)
            {
                if(table_[index].key_==key)
                {
                    return true;
                }
                index=(index+(size_t)1)%capacity_;
            }
            return false;
        }

        value_type operator [] (const key_type &key)
        {
            size_t index=hash(key);
            while(table_[index].is_occupied_)
            {
                if(table_[index].key_==key)
                {
                    return table_[index].value_;
                }
                index=(index+(size_t)1)%capacity_;
            }
            std::cout<<"key not present\n";
            return 0;
        }
        void remove(const key_type &key)
        {
            size_t index=hash(key);
            while(table_[index].is_occupied_)
            {
                if(table_[index].key_==key)
                {
                    table_[index].is_occupied_=false;
                    size_--;
                    return;
                }
                index=(index+(size_t)1)%capacity_;
            }
        }

        size_t size() const noexcept
        {
            return size_;
        }
    };
}

int main()
{
    ns::Open_Hash_table<int,double> ht;
    ht.insert(1,5);
    ht.insert(2,7.3);
    std::cout<<ht.size()<<std::endl;
    std::cout<<ht.contains(1)<<std::endl;
    std::cout<<ht.contains(3)<<std::endl;
    std::cout<<ht[2]<<std::endl;
    ht.insert(1,-1);
    std::cout<<ht.size()<<std::endl;
    std::cout<<ht[1]<<std::endl;
    ht.remove(1);
    std::cout<<ht.size()<<std::endl;
    std::cout<<ht[1]<<std::endl;
    ht.remove(1);
    std::cout<<ht.size()<<std::endl;
}