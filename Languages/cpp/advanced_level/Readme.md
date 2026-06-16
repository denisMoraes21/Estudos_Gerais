# Exercises

## 🧮 1. Plugin system with abstract classes

Develop a plugin system where each plugin inherits from an abstract class "Plugin". The base class should have a pure 
method "execute()". Allow dynamic plugin loading (simulate like objects in the code).

## 🔄 2. Generic sort generic functions using templates 

Make a generic function "sortDescending" using templates that can accept any type of vector (for example, "vector<int>", 
"vector<double>", etc.). Don't use "std::sort" directly - implement your own algorithm (eg: quicksort or merge sort).

## 🔁 3. Bank simulator with threads

Simulate a banking system where multiple clients (threads) access and modify a shared bank account. Use std::mutex to 
avoid race conditions.

## 📦 4. Resource manager with smart pointers

Implement a resource manager (for example, files, network connections) using "std::unique_ptr" and "std::shared_ptr", 
ensuring automatic resources cleanup.

## 🧮 5. Event system with callbacks

Make an event system where the "listeners" can subscribe using lambda functions or "std::function", and an event can 
notify listeners with specific parameters.

## 🧾 6. Arithmetic expressions analyser

Create a parser that receive an expression as a string (ex: "3 + 4 * (2 - 1)") and returns a result. Use stacks and 
trees expressions. Handle operator precedence correctly;

## 📁 7. Logging system with operator overloading

Implement a class "Logger" that overloading "<<" to allow message chaining (like "std::count") and save logs with 
a timestamp.

## 🧮 8. A algorithm (A-Star) for shortest path

Implement a A* algorith to find a shortest path in a two-dimensional with obstacles. Allow setting heuristics with 
Manhattan and Euclidian.

## 🎲 9. Custom container with iterators

Make your own data structure (ex: "MyList") that implements a compatible iterator with "for(auto el : list)". Use 
pointers "std::unique_ptr" for the node structure;

## 🧹 10. Json file interpreter

Implement a simple JSON parser that can understand objects and arrays containing integers, strings and booleans. 
Don't use external libraries. The final structure should allow key and index access;




