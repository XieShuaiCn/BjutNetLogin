/* */

function Component()
{
    // constructor
    //component.loaded.connect(this, Component.prototype.loaded);
    //if (!installer.addWizardPage(component, "Addition", QInstaller.ReadyForInstallation))
    //    console.log("Could not add the 'Addition' page.");
}

Component.prototype.isDefault = function()
{
    // select the component by default
    return true;
}

Component.prototype.createOperations = function()
{
    try {
        // call the base create operations function
        component.createOperations();
    } catch (e) {
        console.log(e);
    }
}

/*
Component.prototype.loaded = function ()
{
    var page = gui.pageByObjectName("Addition");
    if (page != null) {
        console.log("Connecting the dynamic page entered signal.");
        //page.entered.connect(Component.prototype.dynamicPageEntered);
    }
}
*/

Component.prototype.createOperations = function()
{
  // call default implementation to actually install README.txt!
  component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/BjutNetLogin.exe", "@StartMenuDir@/北工大网关登录器.lnk",
              "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/logo.ico",
              "description=运行北工大网关登录器");
        component.addOperation("CreateShortcut", "@TargetDir@/uninstall.exe", "@StartMenuDir@/卸载.lnk",
                "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/uninstall.exe",
                "description=卸载北工大网关登录器");
        component.addOperation("CreateShortcut", "@TargetDir@/BjutNetLogin.exe", "@DesktopDir@/北工大网关登录器.lnk",
              "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/logo.ico",
              "description=运行北工大网关登录器");
        //component.addOperation("CreateShortcut", "@TargetDir@/BjutNetLogin.exe", "@UserStartMenuProgramsPath@/Startup/BjutNetLogin.lnk",
        //      "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/BjutNetLogin.exe",
        //      "description=运行北工大网关登录器");
        //component.addOperation("Settings", "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        //    "set", "BjutNetLogin", "@TargetDir@/BjutNetLogin.exe");
        component.addOperation("GlobalConfig", "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
            "BjutNetLogin", "@TargetDir@/BjutNetLogin.exe");
    }
    else{
        component.addOperation("CreateDesktopEntry", "xieshuai-BjutNetLogin.desktop",
              "Version=@Version@ Type=Application Name=BjutNetLogin Icon=@TargetDir@/logo.ico Exec=@TargetDir@/BjutNetLogin.sh Comment=北京工业大学网关登录器。账号登陆，在线保持。 Categories=Applications;Tools; Terminal=false StartupWMClass=xieshuai-BjutNetLogin");
    }
}
