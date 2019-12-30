#define APPName "�Զ�����ģ��"
#define UninstallName "���Զ�����ģ��-ж��"
#define APPDIRName "public"
#define APPVerName "�Զ�����ģ��"
#define APPPublisher "��������Ҽ�ƶ�����ϵͳ���޹�˾"
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
AppCopyright=��Ȩ���� (C) 2019-2021 ��������Ȩ��.
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
; �������
Source: "{#APPDIRName}\*"; DestDir:{code:GetDefaultDir}; Flags: ignoreversion recursesubdirs createallsubdirs;

[Registry]

[Dirs]

;���������ݷ�ʽ
[Tasks]

[Icons]

[Run]

[Code]
//���崰��
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

//��ȡ��װ·��
function GetDefaultDir(Param: String): String;
begin
  Result := configDir.Values[0];
end;
//ftp�����ж��������ݴ���������һ��
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
//ftp���ý������ʹ�ú���
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
//�û��Զ�������ж�
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
//������ť
procedure onSkipButtonClick(Sender: TObject);
begin
  configUserInfo.Values[0]:='';
  configUserInfo.Values[1]:='';
  WizardForm.NextButton.OnClick(nil);
end;
//��ʼ������
procedure InitializeWizard();
var
  index:Integer;
begin
  //��װĿ¼
  configDir:=CreateInputDirPage(wpWelcome,
  '��װѡ��', '��ѡ����Ҫ��װ�Զ�����ģ��Ĺ���',
  'ѡ��װ·����Ȼ�󵥻���һ����',False,'');
  configDir.Add('��װ·��:');
  configDir.Values[0]:=ExpandConstant('D:/');
  //ftp���ô���
	configPage:=CreateCustomPage(wpInfoBefore, 'ftp����', '����ftp���·�������ص�����');
  addEditToConfigPage('IP:');
  addEditToConfigPage('�˿ں�:');
	addEditToConfigPage('�û���:');
	addEditToConfigPage('����:');
	addEditToConfigPage('���°�����ftpURL:');
	addEditToConfigPage('������Ƶ��(s):');
	editArray[1].Text:='21';
	editArray[5].Text:='60';
	//�û��Զ���
  configUserInfo:=CreateInputFilePage(wpUserInfo,
  '�û��Զ���', '���Զ�����������',
  '��ѡ���Զ����������');
  configUserInfo.Add('�Զ���ͼ��:','ͼ��(*.ico)|*.ico','*.ico');
  configUserInfo.Add('�Զ�����¶���:','ͼ��(*.gif)|*.gif','*.gif');
  configUserInfo.Edits[0].OnChange:=@isHaveInfo;
  configUserInfo.Edits[1].OnChange:=@isHaveInfo;
  skipButton:=TNewButton.Create(WizardForm);
  skipButton.Parent := WizardForm;
  skipButton.Left := 10;
  skipButton.Top := WizardForm.NextButton.Top;
  skipButton.Caption := '����';
  skipButton.OnClick := @onSkipButtonClick;
  skipButton.Visible := False;
end;
//��ҳ�ж�
procedure CurPageChanged(CurPageID: Integer);
begin
  Log(IntToStr(CurPageID)+IntToStr(wpReady));
  if CurPageID=configPage.ID then
  begin
    Key_Form_KeyChange(editArray[0]);
  end;
  skipButton.Visible := CurPageID=configUserInfo.ID;
end;
//�����ж�
procedure CurStepChanged(CurStep: TSetupStep);
  var
  fileName,tempStr,configDirName:String;
  svArray: TArrayOfString;
  nLines,i:Integer;
  iconFile:String;
  gifFile:String;
begin
	if CurStep=ssinstall then
	//��װǰִ��
	begin
  end;
	if CurStep=ssPostinstall then
	//�����ļ���ִ��
	begin
    //��������Ŀ¼
	  configDirName:=configDir.Values[0]+ExpandConstant('\config');
    CreateDir(configDirName);
    //�����ļ�
		fileName := configDirName+ExpandConstant('\configUpdater.ini');
		SaveStringsToFile(fileName, svArray, false);
		SetIniString('Service','Host',editArray[0].Text,fileName);
		SetIniString('Service','Port',editArray[1].Text,fileName);
		SetIniString('Service','User',editArray[2].Text,fileName);
		SetIniString('Service','Password',editArray[3].Text,fileName);
		SetIniString('Service','UpdateWorkDir',editArray[4].Text,fileName);
		SetIniString('UpdateConfig','UpdateTime',editArray[5].Text,fileName);
		//�Զ����ļ�����
		if(Length(configUserInfo.Values[0])>0) then
		begin
      iconFile:=configDir.Values[0]+ExpandConstant('\AutoUpdaterByFtp.ico');
      gifFile:=configDir.Values[0]+ExpandConstant('\AutoUpdaterByFtp.gif');
      FileCopy(configUserInfo.Values[0],iconFile,False);
      FileCopy(configUserInfo.Values[1],gifFile,False);
		end;


	end;
	if CurStep=ssDone then
	//��װ��ɺ�ִ��
	begin
	end;
end;
[UninstallRun]