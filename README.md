To compile this code run:

g++ proxy.cpp

Steps to run this code:

Set up your web browser to see the proxy using IP: 127.0.0.1 and Port: 8080

Next run the transparent proxy by typing <./a.out f> into the terminal while in the directory of the executable.
Make sure to type the f or you will get a segmentation fault

Next to run the froxy type <./a.out frog> into the terminal

Occasionally, the request for the ponds webpage may causes issues and not let any more requests in. If this happens simply close
the browser and manually shut down the proxy with control-c. Then re-run the proxy with the steps above.

Features:

The proxy will handle any http request to the website pages.cpsc.ucalgary.ca/~jcleahy/
and any of the links on this website. The proxy does not send or handle any images png or jpeg 
(i.e you will not see any images period). 

The froxy will change any instance of frog to fred before sending the html to the web browser but
does not show any frog images.

Any http request not to the specified website above will not work. Additionally, passing too many requests in
a short period of time will crash the proxy.

Testing:

All the testing was done via lab computers, vm linux, and on my macbook (both at home and at university).
During testing the proxy seemed to be most stable on mac using safari as firefox on linux tended to send more random
requests that sometimes crash the code. 

Running curl for requests seemed to function way better than the web browsers allowing proper data transfer everytime.

Note: sometimes it takes a little bit for the website to load (be patient)
