Q. Does one hashing collision resolution work better than the other? Consider the datasets we used and a much larger dataset with 1 million records. Explain your answer.

1. In this case, chaining is the much better collision resolution technique. Because our dataset is small enough, the linked lists (which take O(n) to traverse) do not become too long. With a larger data set, the linked lists might become so long that it is more ideal to use another resolution method. Thought, with a better hash function, like the one in the C++ standard library, this can be avoided.

Q. We used the same BST for both hashing mechanisms. What alternative data structure would you advise? Why?


2. I would advise using a hash table, using the professor's name or ID as the key. With a good hash function, this would make searching professors O(1) time instead of O(log(n)).

Q. Explain a few ways you could improve the application.

3. The obvious answer is a UI. Using the terminal is really not ideal for any modern program. As for the program itself, using a better hash function is again a very obvious improvement. The C++ standard library has a hash library that is very easy to use, or any external hash function. Also, not using a BST would be an easy improvement, as none of the professors need to be "connected" to each other, so using a tree does not really make sense. Using smart pointers would also be an improvement, as to not need to do manual garbage collection. Also, it would be nice for users to be able to manually edit things once in the hash table in a way other than adding it to the csv file. 
