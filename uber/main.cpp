#include<bits/stdc++.h>
using namespace std;

// started on 10:18
class SegmentTree{
    vector<int> ST;
    public:
    SegmentTree(vector<int> &arr){
        int n=arr.size();
        ST.resize(4*n+1);
        build(0,0,n-1,arr);
    }
    void build(int node, int low, int high, vector<int> &arr){
        if( low==high){
            ST[node]=arr[low];
            return ;
        }
        int mid=(low+high)/2;
        build(node*2+1, low, mid, arr);
        build(node*2+2, mid+1, high, arr);
        return;
    }
    int query(int node, int low,int high, int l, int r){
        // no overlap
        if(r<low || high<l)return INT_MAX;
        // full
        if(l<=low && high<=r)return ST[node];
        int mid=(low+high)/2;
        int left=query(node, low, mid, l, r);
        int right=query(node, mid+1, high, l,r );
        return min(left, right);
    }
};