# MIT-CG6.837-2004 
  
课程网站  
6.837 Intro to Computer Graphics, Fall 2004
http://groups.csail.mit.edu/graphics/classes/6.837/F04/index.html  

Assignment 0: Iterated Function Systems 
迭代函数系统 
产生随机的点，经过矩阵变换，保存到图片中。

Assignment 1: Ray Casting
光线投射 
正交相机+球体

Assignment 2: Transformations & Additional Primitives  
变换 & 基本图元
疑问：
transform.cpp
//direction.Normalize();
Ray r2(origin, direction);
对direction进行单位化，scene_16就出错

Assignment 3: OpenGL & Phong Shading
OpenGL和Phong着色
疑问：
1.PhongMaterial::shade()
使用getDiffuseColor()与使用diffuseColor不同，为什么？
2.sphere的paint() 理解不深。

Assignment 4: Shadows, Reflection & Refraction
阴影，反射和折射 
1.rayTracer 递归计算，返回值为当前点的phong颜色与递归下一点的phong颜色
2.使用了opengl的sphere::paint()函数

Assignment 5: Voxel Rendering
体素渲染
未完全实现

Assignment 6: Grid Acceleration & Solid Textures
网格加速和纹理
未实现

Assignment 7: Supersampling & Antialiasing 
采样和抗锯齿  
未实现

Assignment 8: Curves & Surfaces 
曲线和曲面  
疑问：
1.为什么代码里 Bezier转Bspline 矩阵G里控制点是行向量时，结果才正确，课件里说明控制点输入应该是列向量
2.Bezier使用了递归计算，取消了pow运算，那Bspline可以简化吗

Assignment 9: Particle Systems
质点系统 
1.refresh影响质点update的速度，dt影响质点运动状态，dt越小，pos，vel变化越小。
2.midPointIntegrator 比 hose 更精确，circle.txt可以变成圆
3.为什么 refresh dt<0.01时 不显示？
4.修改密度by desired_num_particles 
5.未实现 wind

参考：
https://github.com/Birdy-C/MIT-6-837
https://github.com/Aesma/CG-Project3-3
