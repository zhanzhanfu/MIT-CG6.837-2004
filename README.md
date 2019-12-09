# MIT-CG6.837-2004 

### reviewing 正在重写中，会比较慢，之前的23489 差不多能用。

课程网站 

6.837 Intro to Computer Graphics, Fall 2004

http://groups.csail.mit.edu/graphics/classes/6.837/F04/index.html 

环境配置：

- Win10 + Clion
- src 文件夹包括课程提供的 .h .C 和自己写的.h .cpp
- out 文件夹包括.exe .txt .tga

### 0. Iterated Function Systems

产生 num 个随机点，经过 iters 次矩阵（随机选取）变换，保存到图片中对应位置。 

### 1. Ray Casting

这里使用 Algebraic 和 Geometric 计算交点。

**注意点1：**

不要 delete 同一片内存两次，不需要在 ~Object3D() 中 delete material; 因为 material 申请内存的操作不在这个类中，由其他类负责。

```
class Object3D {
//virtual ~Object3D() {delete material;}  error
virtual ~Object3D() {}  //right

Material *material;
};
//因为scene_parser.C中 ~SceneParser已经 delete materials[i]
//这个错误在clion中只会返回异常: 0xC0000005，而在vs中会指向错误的地方，当clion中返回值!=0时，又找不到出错的地方，可以在vs中找。
```

**注意点2：**

image 对应于 （0,200）and 比例（0,1）

OrthographicCamera 的 center 在（0,0），因此产生的光线对应于 （-1,1）* size

**注意点3：**

error: multiple definition of 函数多次定义

解决办法一：把函数定义写到 .cpp；

解决办法二：在 .h文件中，把定义与声明写在一起，即把函数定义写在类内。

**注意点4：**

在使用 Geometric 法计算交点时，为了得到与样例相符的图片，忽略 t 是否在视线后面，即 camera center 是否在球体内部，永远取 t = min(t1, t2)

参考：

https://github.com/Birdy-C/MIT-6-837

https://github.com/Aesma/CG-Project3-3

