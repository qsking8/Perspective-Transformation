# 透视变换（Perspective Transformation）
在应用中，需要用到OpenCV的两个函数getPerspectiveTransform()和warpPerspective()两个函数。

实例:
![image](https://github.com/qsking8/Perspective Transformation/master/demo.gif)

```c++
Mat getPerspectiveTransform(InputArray src, InputArray dst, int solveMethod = DECOMP_LU)
```

        参数描述

        参数 src：源图像四边形的4个顶点坐标。

        参数 dst：目标图像对应四边形的4个顶点坐标。

        参数 solveMethod：传递给cv::solve(#DecompTypes)的计算方法，默认是DECOMP_LU，一般不用输入此参数。

        返回值：Mat型变换矩阵，可直接用于warpPerspective()函数
```c++
void warpPerspective(
	InputArray src,
	OutputArray dst,
	InputArray M,
	Size dsize,
	int flags=INTER_LINEAR,
	int borderMode = BORDER_CONSTANT, 
	const Scalar& borderValue = Scalar());
```
        参数描述

        参数src：输入图像。

        参数dst：输出图像，需要初始化一个空矩阵用来保存结果，不用设定矩阵尺寸。

        参数M：3x3的转换矩阵。

        参数dsize：输出图像的大小。

        参数flags：设置插值方法。默认为INTER_LINEAR表示双线性插值，INTER_NEAREST表示最近邻插值，WARP_INVERSE_MAP表示M作为反转转换 (dst->src) 。

        参数borderMode：像素外推方法，默认为BORDER_CONSTANT，指定常数填充。翻阅官方文档发现还有一个选项是BORDER_REPLICATE。

        参数borderValue：常数填充时边界的颜色设置，默认是(0,0,0)，表示黑色。这就是为什么透视变换后图片周围是黑色的原因。这里需要注意的是类型为Scalar (B, G, R)。

