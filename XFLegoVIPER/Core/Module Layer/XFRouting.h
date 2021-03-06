//
//  XFWireframe.h
//  XFLegoVIPER
//
//  Created by yizzuide on 15/12/22.
//  Copyright © 2015年 yizzuide. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "XFWireFramePort.h"
#import "XFUIOperatorPort.h"
#import "XFLegoMarco.h"
#import "XFUIBus.h"
#import "XFModuleAssembly.h"

// 快速获取当前模块视图
#define MainActivity LEGORealInterface(self.realInterface)

// 获取一个类的类
#define XF_Class_(Class) [Class class]

/* --------------------------------- *模块组装* --------------------------------- */
/* ------------------------------ 自定义手动组装方式 ------------------------------ */
// 快速注入模块成分-导航方式
#define XF_InjectModuleWith_Nav(_NavigatorClass_,_ActivityClass_,_PresenterClass_,_InteractorClass_,_DataManagerClass_) \
+ (void)load \
{ \
    [XFRoutingReflect inspectModulePrefixFromClass:self]; \
} \
+ (instancetype)assembleRouting \
{ \
    return [[super routing].assembly buildModulesAssemblyWithActivityClass:_ActivityClass_ \
                                                   navigatorClass:_NavigatorClass_ \
                                                   presenterClass:_PresenterClass_ \
                                                  interactorClass:_InteractorClass_ \
                                                 dataManagerClass:_DataManagerClass_]; \
}

// 快速注入模块成分-子界面方式（全自定义组装常用方式）
#define XF_InjectModuleWith_Act(_ActivityClass_,_PresenterClass_,_InteractorClass_,_DataManagerClass_) \
XF_InjectModuleWith_Nav(nil,_ActivityClass_,_PresenterClass_,_InteractorClass_,_DataManagerClass_)

// 快速注入模块成分-ib方式
#define XF_InjectModuleWith_IB(ibSymbol,_PresenterClass_,_InteractorClass_,_DataManagerClass_) \
+ (void)load \
{ \
    [XFRoutingReflect inspectModulePrefixFromClass:self]; \
} \
+ (instancetype)assembleRouting \
{ \
    return [[super routing].assembly buildModulesAssemblyWithIB:ibSymbol presenterClass:_PresenterClass_ interactorClass:_InteractorClass_ dataManagerClass:_DataManagerClass_]; \
}


/* --------------------------------- 全自动组装方式 --------------------------------- */
// 自动组装模块成分基本方法（不适于直接使用）
#define XF_AutoAssemblyModule(NavName,IBSymbol,ShareDataManagerName) \
+ (void)load \
{ \
    [XFRoutingReflect inspectModulePrefixFromClass:self]; \
} \
+ (instancetype)assembleRouting \
{ \
    return [[super routing].assembly autoAssemblyModuleWithNav:NavName ibSymbol:IBSymbol shareDataManagerName:ShareDataManagerName]; \
}
// 自动组装一个无Nav的模块成分
// 有共享其它模块DataManager的
#define XF_AutoAssemblyModule_ShareDM(ShareDataManagerName) XF_AutoAssemblyModule(nil,nil,ShareDataManagerName)
// 无共享其它模块DataManager（全自动组装方式中最常用）
#define XF_AutoAssemblyModule_Fast XF_AutoAssemblyModule_ShareDM(nil)

// 自动组装一个有Nav模块成分
// 有自定义导航和共享其它模块DataManager的
#define XF_AutoAssemblyModuleWithNav_ShareDM(NavName,ShareDataManagerName) XF_AutoAssemblyModule(NavName,nil,ShareDataManagerName)
// 有自定义导航和无共享其它模块DataManager的
#define XF_AutoAssemblyModuleWithNav(NavName) XF_AutoAssemblyModuleWithNav_ShareDM(NavName,nil)

// 根据项目模块前辍自动组装一个有Nav模块成分（全自动组装方式中较常用）
#define XF_AutoAssemblyModuleWithNav_Fast \
+ (void)load \
{ \
    [XFRoutingReflect inspectModulePrefixFromClass:self]; \
} \
+ (instancetype)assembleRouting \
{ \
    return [[super routing].assembly autoAssemblyModuleWithPrefixNav]; \
}

// 自动组装一个IBSymbol的模块成分
#define XF_AutoAssemblyModuleFromIB_ShareDM(IBSymbol,ShareDataManagerName) XF_AutoAssemblyModule(nil,IBSymbol,ShareDataManagerName)
#define XF_AutoAssemblyModuleFromIB(IBSymbol) XF_AutoAssemblyModuleFromIB_ShareDM(IBSymbol,nil)


// 自动组装一个基于其它模块成分类型的模块（全自动组装方式中常用）
#define XF_AutoAssemblyModuleFromShareModuleName(shareModuleName) \
+ (instancetype)assembleRouting \
{ \
    return [[super routing].assembly autoAssemblyModuleFromShareModuleName:shareModuleName]; \
}



/* --------------------------------- *组件的跳转* --------------------------------- */
/* --------------------------------- 组件名方式 --------------------------------- */
// Push一个组件宏
#define XF_PUSH_Component_(ComponentName,ExecuteCode) \
XF_Define_Weak \
[self.uiBus pushComponent:ComponentName intent:self.uiOperator.intentData customCode:^(Activity *nextInterface) { \
    XF_Define_Strong \
    ExecuteCode \
    [self self]; \
}];
// 快速Push一个组件
#define XF_PUSH_Component_Fast(ComponentName) \
XF_PUSH_Component_(ComponentName,{})

// Present一个组件宏
#define XF_Present_Component_(ComponentName,ExecuteCode) \
XF_Define_Weak \
[self.uiBus presentComponent:ComponentName intent:self.uiOperator.intentData customCode:^(Activity *nextInterface) { \
    XF_Define_Strong \
    ExecuteCode \
    [self self]; \
}];
// 快速Present一个组件
#define XF_Present_Component_Fast(ComponentName) \
XF_Present_Component_(ComponentName,{})

/* --------------------------------- URL组件方式 --------------------------------- */
// Push一个URL组件
#define XF_PUSH_URLComponent_(urlString,ExecuteCode) \
XF_Define_Weak \
[self.uiBus openURLForPush:urlString customCode:^(Activity *nextInterface) { \
    XF_Define_Strong \
    ExecuteCode \
    [self self]; \
}];
// 快速Push一个URL组件
#define XF_PUSH_URLComponent_Fast(urlString) \
XF_PUSH_URLComponent_(urlString,{})

// Present一个URL组件
#define XF_Present_URLComponent_(urlString,ExecuteCode) \
XF_Define_Weak \
[self.uiBus openURLForPresent:urlString customCode:^(Activity *nextInterface) { \
    XF_Define_Strong \
    ExecuteCode \
    [self self]; \
}];
// 快速Present一个URL组件
#define XF_Present_URLComponent_Fast(urlString) \
XF_Present_URLComponent_(urlString,{})

@interface XFRouting : NSObject <XFWireFramePort>

/**
 *  模块组装器
 */
@property (nonatomic, strong, readonly) XFModuleAssembly *assembly;
/**
 *  UI总线
 */
@property (nonatomic, strong, readonly) XFUIBus *uiBus;
/**
 *  事件总线
 */
@property (nonatomic, strong, readonly) XFEventBus *eventBus;

/**
 *  事件处理层（Presenter）
 */
@property (nonatomic, weak, readonly) __kindof id<XFUIOperatorPort> uiOperator;

/**
 *  上一个关联的模块路由
 */
@property (nonatomic, strong, readonly) __kindof XFRouting *previousRouting;
/**
 *  下一个关联的模块路由
 */
@property (nonatomic, strong, readonly) __kindof XFRouting *nextRouting;

/**
 *  父路由
 */
@property (nonatomic, weak, readonly) __kindof XFRouting *parentRouting;

/**
 *  当前是否是子路由
 */
@property (nonatomic, assign, getter=isSubRoute) BOOL subRoute;

/**
 *  所有子路由
 */
@property (nonatomic, strong, readonly) NSMutableArray<__kindof XFRouting *> *subRoutings;

/**
 *  返回初始路由对象（默认包含路由的组装器、UI总线、Event总线）
 *
 *  @return 路由
 */
+ (instancetype)routing;

/**
 *  从路由组装当前模块各层（这是一个抽象方法，子类必需覆盖实现组装方式）
 *
 *  @return 组装完成的路由
 */
+ (instancetype)assembleRouting;

/**
 *  添加子路由
 *
 *  @param subRouting 子路由
 *  @param asChild    是否自动添加子控制器
 *
 */
- (void)addSubRouting:(XFRouting *)subRouting asChildViewController:(BOOL)asChild;

/**
 *  获得当前真实存在的视图
 *
 *  @return 视图
 */
- (__kindof id<XFUserInterfacePort>)realInterface;
/**
 *  获得包装当前视图的导航
 *
 *  @return 导航
 */
- (__kindof UINavigationController *)realNavigator;
@end
