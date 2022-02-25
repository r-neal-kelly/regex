# regex

I have a fascination with regex engines and searching text. I want my regex engine to handle ancient languages as well as modern, so it has to support various types of unicode out of the box, and a regex flavor that facilitates use of common unicode points like [Greek], [Hebrew], [Latin], etc.

This is a prototype project. Everything in it, encluding the Unicode string has already been ported to C++ in [nkr](https://github.com/r-neal-kelly/nkr). I may come back to this project when I start to fully design the regex engine proper after I get all the necessary parts in place that it depends on.

I chose to prototype in C because it such a clean language and allows for clarity of though in design.
