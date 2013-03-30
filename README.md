Timothy Linell Bonnette,
University of Southern Mississippi
Department of Computer Science

This research uses OpenCV and the Microsoft Kinect to automatically count boards in a bunk of lumber. It is being actively developed for my undergraduate thesis.

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