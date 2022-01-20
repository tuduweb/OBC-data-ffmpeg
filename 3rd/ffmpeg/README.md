## FFMPEG

当前环境：win10

下载方法：

在 https://www.gyan.dev/ffmpeg/builds/ 网站下，下载ffmpeg-release-full-shared.7z版本，亦可达到之前方法的效果。

后期更换成直接编译，包含。

#### 注意事项

需要把`.dll`放到程序目录下!

其它

 > 在音频视频编解码中，经常使用ffmpeg库。平时都是使用的动态库，但是动态库有一个缺点，就是程序运行时才进行加载。如果你需要经常移植的情况下，每次都需要重新的编译动态库，比较麻烦。并且，如果你使用的库只有你自己的程序使用时，那么系统只会有一个副本，所以动态库的优点并没有体现出来，倒不如使用静态库省事。