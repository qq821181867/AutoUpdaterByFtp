#define APPName "自动更新模块"
#define UninstallName "可自动更新模块-卸载"
#define APPDIRName "public"
#define APPVerName "自动更新模块"
#define APPPublisher "深圳市零壹移动互联系统有限公司"
#define APPPublisherURL "http://www.0easy.com/"

[setup]
AppName={#APPName}
AppVerName={#APPVerName}
AppPublisher={#APPPublisher}
AppPublisherURL={#APPPublisherURL}
AppSupportURL={#APPPublisherURL}
AppUpdatesURL={#APPPublisherURL}
DefaultDirName={pf}
DisableDirPage=yes
DefaultGroupName=
DisableProgramGroupPage=no
OutputDir=.\setup
OutputBaseFilename={#APPVerName}
Compression=zip
SolidCompression=yes
VersionInfoVersion=1.0
AppCopyright=版权所有 (C) 2019-2021 保留所有权限.
AppComments=
Encryption=no
SetupIconFile={#APPDIRName}\AutoUpdaterByFtp.ico
Uninstallable=no
UninstallDisplayName={#UninstallName}
UninstallDisplayIcon={#APPDIRName}\AutoUpdaterByFtp.ico
ChangesEnvironment = true

[Languages]
Name: "chinese"; MessagesFile: "compiler:Default.isl"

[Messages]
BeveledLabel={#APPName}

[Files]
; 打包内容
Source: "{#APPDIRName}\*"; DestDir:{code:GetDefaultDir}; Flags: ignoreversion recursesubdirs createallsubdirs;

[Registry]

[Dirs]

;创建桌面快捷方式
[Tasks]

[Icons]

[Run]

[Code]
//定义窗口
var
configPage:TwizardPage;
LbHost: TNewStaticText;
editHost:TEdit;
LbPort: TNewStaticText;
editPort:TEdit;
LbUser: TNewStaticText;
editUser:TEdit;
LbPassword: TNewStaticText;
editPassword:TEdit;
LbUpdateWorkDir: TNewStaticText;
editUpdateWorkDir:TEdit;
LbUpdateTime: TNewStaticText;
editUpdateTime:TEdit;
lbArray:array of TNewStaticText;
editArray:array of TEdit;

configDir:TInputDirWizardPage;

configUserInfo:TInputFileWizardPage;
skipButton:TNewButton;

//获取安装路径
function GetDefaultDir(Param: String): String;
begin
  Result := configDir.Values[0];
end;
//ftp界面判断输入内容存在允许下一步
procedure Key_Form_KeyChange(Sender: TObject);
var
  isEnabled:Boolean;
  index:Integer;
begin
  isEnabled:=True;
  for index:=0 to GetArrayLength(editArray)-1 do
  begin
    if(Length(editArray[index].Text)<=0) then
    begin
      isEnabled:=False;
      WizardForm.NextButton.Enabled := isEnabled;
      exit;
    end;
  end;
	WizardForm.NextButton.Enabled := isEnabled;
end;
//ftp配置界面添加使用函数
function addEditToConfigPage(Param: String):Boolean;
var
  lable:TNewStaticText;
  edit:TEdit;
  index:Integer;
begin
  index:=GetArrayLength(editArray);
  lable:=TNewStaticText.Create(configPage);
  lable.Left := ScaleX(5);
  lable.Top := ScaleY(0);
	lable.Width := ScaleX(250);
	lable.Height := ScaleY(20);
	lable.Caption := Param;
	lable.Parent := configPage.Surface;
	edit:=TEdit.Create(configPage);
	edit.Width:=ScaleX(410);
	edit.Top := lable.Top+lable.Height;
	edit.Text :='';
	edit.Parent:=configPage.Surface;
	edit.OnChange := @Key_Form_KeyChange;
	//
  SetArrayLength(lbArray, index+1);
  SetArrayLength(editArray, index+1);
  lbArray[index]:=lable;
	editArray[index]:=edit;
	//
	if(index>0) then
	begin
    lable.Top := editArray[index-1].Top+editArray[index-1].Height;
    edit.Top := lable.Top+lable.Height;
	end;

  Result:=True;
end;
//用户自定义界面判断
procedure isHaveInfo(Sender: TObject);
var
  index:Integer;
  isEnabled:Boolean;
begin
  isEnabled:=True;
  for index:=0 to 2-1 do
  begin
    if(Length(configUserInfo.Values[index])<=0) then
    begin
      isEnabled:=False;
      WizardForm.NextButton.Enabled := isEnabled;
      exit;
    end;
    WizardForm.NextButton.Enabled := isEnabled;
  end;
end;
//跳过按钮
procedure onSkipButtonClick(Sender: TObject);
begin
  configUserInfo.Values[0]:='';
  configUserInfo.Values[1]:='';
  WizardForm.NextButton.OnClick(nil);
end;
//初始化界面
procedure InitializeWizard();
var
  index:Integer;
begin
  //安装目录
  configDir:=CreateInputDirPage(wpWelcome,
  '安装选择', '请选择需要安装自动更新模块的工程',
  '选择安装路径，然后单击下一步。',False,'');
  configDir.Add('安装路径:');
  configDir.Values[0]:=ExpandConstant('D:/');
  //ftp配置窗口
	configPage:=CreateCustomPage(wpInfoBefore, 'ftp配置', '用于ftp更新服务器相关的配置');
  addEditToConfigPage('IP:');
  addEditToConfigPage('端口号:');
	addEditToConfigPage('用户名:');
	addEditToConfigPage('密码:');
	addEditToConfigPage('更新包所在ftpURL:');
	addEditToConfigPage('检测更新频率(s):');
	editArray[1].Text:='21';
	editArray[5].Text:='60';
	//用户自定义
  configUserInfo:=CreateInputFilePage(wpUserInfo,
  '用户自定义', '可自定义的相关内容',
  '可选择自定义或者跳过');
  configUserInfo.Add('自定义图标:','图标(*.ico)|*.ico','*.ico');
  configUserInfo.Add('自定义更新动画:','图标(*.gif)|*.gif','*.gif');
  configUserInfo.Edits[0].OnChange:=@isHaveInfo;
  configUserInfo.Edits[1].OnChange:=@isHaveInfo;
  skipButton:=TNewButton.Create(WizardForm);
  skipButton.Parent := WizardForm;
  skipButton.Left := 10;
  skipButton.Top := WizardForm.NextButton.Top;
  skipButton.Caption := '跳过';
  skipButton.OnClick := @onSkipButtonClick;
  skipButton.Visible := False;
end;
//翻页判断
procedure CurPageChanged(CurPageID: Integer);
begin
  Log(IntToStr(CurPageID)+IntToStr(wpReady));
  if CurPageID=configPage.ID then
  begin
    Key_Form_KeyChange(editArray[0]);
  end;
  skipButton.Visible := CurPageID=configUserInfo.ID;
end;
//步骤判断
procedure CurStepChanged(CurStep: TSetupStep);
  var
  fileName,tempStr,configDirName:String;
  svArray: TArrayOfString;
  nLines,i:Integer;
  iconFile:String;
  gifFile:String;
begin
	if CurStep=ssinstall then
	//安装前执行
	begin
  end;
	if CurStep=ssPostinstall then
	//复制文件后执行
	begin
    //创建配置目录
	  configDirName:=configDir.Values[0]+ExpandConstant('\config');
    CreateDir(configDirName);
    //配置文件
		fileName := configDirName+ExpandConstant('\configUpdater.ini');
		SaveStringsToFile(fileName, svArray, false);
		SetIniString('Service','Host',editArray[0].Text,fileName);
		SetIniString('Service','Port',editArray[1].Text,fileName);
		SetIniString('Service','User',editArray[2].Text,fileName);
		SetIniString('Service','Password',editArray[3].Text,fileName);
		SetIniString('Service','UpdateWorkDir',editArray[4].Text,fileName);
		SetIniString('UpdateConfig','UpdateTime',editArray[5].Text,fileName);
		//自定义文件复制
		if(Length(configUserInfo.Values[0])>0) then
		begin
      iconFile:=configDir.Values[0]+ExpandConstant('\AutoUpdaterByFtp.ico');
      gifFile:=configDir.Values[0]+ExpandConstant('\AutoUpdaterByFtp.gif');
      FileCopy(configUserInfo.Values[0],iconFile,False);
      FileCopy(configUserInfo.Values[1],gifFile,False);
		end;


	end;
	if CurStep=ssDone then
	//安装完成后执行
	begin
	end;
end;
[UninstallRun]