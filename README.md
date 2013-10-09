Timothy Linell Bonnette,
University of Southern Mississippi
Department of Computer Science

This research uses OpenCV and the Microsoft Kinect to automatically count boards in a bunk of lumber. It is being actively developed for my undergraduate thesis.

The current phase of research is directed into using the mean shift algorithm. However, the end goal is to count the number of objects found inside of an image. I am having difficulty figuring out how to do this. It seems like something that is easy enough and is just going over my head. Hey, I suppose that's the whole point of research.

To run this code, I usually end up jumping through rather
a few hoops. You'll need OpenCV and Macports installed, at least. For me to 
run this on my Macbook, I need the following commands:
```
export PKG_CONFIG_PATH=/opt/local/lib/pkgconfig

g++ -bind_at_load `pkg-config --cflags opencv` main.cpp -o main `pkg-config --libs opencv`
```
Sometimes this doesn't exactly work though, for too many silly reasons to even being to understand. I've found that the problem is sometimes that pkgconfig is out of date. In this case, you can update it by
```
sudo port install pkgconfig
```

I suggest making yourself a function to run it for you. Mine is called `doOpenCV` and it *really* helps me out with running the programs.

It's also worth noting that sometimes MacPorts breaks for no good reason. Usually right before your adviser looks at your code. http://charles.lescampeurs.org/2008/08/05/sudo-port-command-not-found this may help.