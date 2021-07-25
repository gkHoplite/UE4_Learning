# Adavanced Way to assigning string
Initialize Array and Load the words, make another Array, check criteria, populating it is takes some waste, you could do it at one funtion

Over 4.25 version, you can use **FFileHelper::LoadFileToStringArrayWithPredicate**
```c++
LoadFileToStringArrayWithPredicate(
    Isograms, 
    *WordListPath, 
    [](const FString& Word) { return Word.Len() < 10; });
```

# Why Lambda can't use member variable
- when objects are initialized, this variable are made that pointing inself

- this pointer was passed as a first arguments used when you call the member function

- Just because Lambda itself, didn't has "this" variable. making IsIsogram and GetValidWords to be static is proper because this function didn't use any member variable, member function

[Question and Answer on Udemy](!https://www.udemy.com/course/unrealcourse/learn/lecture/20626310#questions/12744019)

```c++
class UBullCowCartridge : public UActorComponent
{
    //...
    static bool IsIsogram(const FString& Word);
    // ^ static member function declaration ^
};
```
The function that was declared was a static member function as noted by the function declaration

Which means the lambda now doesn't have to be given an object to call that function on i.e. if it was a non-static member function it would need to do this
```c++
[](const FString& Word) { this->IsIsogram(Word); };
```
But the lambda doesn't have this . Making it a static member function removes the need for an object to call it on. (as such it can't access member variables, which is okay because IsIsogram doesn't use them)



# Template and Static
Templates has its own static member followed by type.

below example show that each type has its own static variable.
``` c++
template <class T> class Test
{  
private:
  T val; 
public:
  static int count;
  Test(){count++;}
};
  
template<class T>
int Test<T>::count = 0;
  
int main()
{
  Test<int> a;  // value of count for Test<int> is 1 now
  Test<int> b;  // value of count for Test<int> is 2 now
  Test<double> c;  // value of count for Test<double> is 1 now
  cout << Test<int>::count   << endl;  // prints 2  
  cout << Test<double>::count << endl; //prints 1
}
```