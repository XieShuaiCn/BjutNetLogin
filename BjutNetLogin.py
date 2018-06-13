import urllib.parse, urllib.request
import re
import threading
import time
import json
import os


class BjutNet (threading.Thread):
    """北京工业大学网关管理类"""

    def __init__(self):
        threading.Thread.__init__(self)
        # 获取这个脚本的路径
        model_path = os.path.split(os.path.realpath(__file__))[0]
        conf_file_etc = "/etc/bjutnet.d/config.json"
        conf_file_home = os.path.join(os.environ['HOME'], ".bjutnet/config.json")
        conf_file_model = os.path.join(model_path, "config.json")
        if os.path.isfile(conf_file_etc):
            conf_file = conf_file_etc
        elif os.path.isfile(conf_file_home):
            conf_file = conf_file_home
        elif os.path.isfile(conf_file_model):
            conf_file = conf_file_model
        else:
            print("配置文件不存在！（%s|%s|%s）" % (conf_file_etc, conf_file_home, conf_file_model))
            return

        with open(conf_file, "r") as f:
            text = f.read()
            json_arr = json.loads(text)
            if "account" in json_arr:
                self._account['account'] = json_arr['account']
            else:
                print("未设置用户名(account)")
                return
            if "password" in json_arr:
                self._account['password'] = json_arr['password']
            else:
                print("未设置密码(password)")
                return
            if "type" in json_arr:
                self._account['type'] = int(json_arr['type'])

    def login(self):
        """登陆北工大网关"""
        # 构造表单
        data = {"DDDDD": self._account['account'], "upass": self._account['password'], "v46s": '1', "v6ip": '', "f4serip": "172.30.201.10", "0MKKey": ""}
        header = {"User-Agent": "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.167 Safari/537.36",
                  "Content-Type": "application/x-www-form-urlencoded"}
        if self._account['type'] == 4:
            url = "https://lgn.bjut.edu.cn"
            data['v46s'] = '1'
        elif self._account['type'] == 6:
            url = "https://lgn6.bjut.edu.cn"
            data['v46s'] = '2'
        elif self._account['type'] == 46:
            url = "https://lgn6.bjut.edu.cn/V6?https://lgn.bjut.edu.cn"
            data['v46s'] = '0'
        else:
            return False, "未知的IP登录类型"
        urldata = urllib.parse.urlencode(data).encode("gbk")
        req = urllib.request.Request(url, data=urldata, headers=header)
        request = urllib.request.urlopen(req)
        if request is None:
            return False, "打开网页失败"
        if request.status > 400:
            return False, "错误码：" + str(request.status)
        content = request.read()
        content = content.decode("gbk")
        html_is_suc = "<title>登录成功窗</title>" in content
        if html_is_suc:
            return True, "登录成功"
        html_is_msg = "<title>信息返回窗</title>" in content
        if html_is_msg:
            html_msg = re.search(r"Msg=(\d*?);", content, re.U).group(1)
            html_msga = re.search(r"msga='(\w*?)';", content, re.U).group(1)
            html_msg = int(html_msg)
            msg = ""
            suc = False
            if html_msg == 1:
                if len(html_msga) > 0:
                    if html_msga == "error0":
                        msg = "本IP不允许Web方式登录"
                    elif html_msga == "error1":
                        msg = "本账号不允许Web方式登录"
                    elif html_msga == "error2":
                        msg = "本账号不允许修改密码"
                    else:
                        msg = html_msga
                else:
                    msg = "账号或密码不对，请重新输入"
            elif html_msg == 2:
                msg = "该账号正在使用中，请您与网管联系"
            elif html_msg == 3:
                msg = "本账号只能在指定地址使用"
            elif html_msg == 4:
                msg = "本账号费用超支或时长流量超过限制"
            elif html_msg == 5:
                msg = "本账号暂停使用"
            elif html_msg == 6:
                msg = "系统缓存已满"
            elif html_msg == 7:
                msg = ""
            elif html_msg == 8:
                msg = "本账号正在使用,不能修改"
            elif html_msg == 9:
                msg = "新密码与确认新密码不匹配,不能修改"
            elif html_msg == 10:
                msg = "密码修改成功"
                suc = True
            elif html_msg == 11:
                msg = "本账号只能在指定地址使用"
            elif html_msg == 14:
                msg = "注销成功"
                suc = True
            elif html_msg == 15:
                msg = "登录成功"
                suc = True
            return suc, msg

    def check(self):
        """检查北工大网关状态"""
        try:
            request = urllib.request.urlopen("https://lgn.bjut.edu.cn")
            if request is None:
                return False, "打开页面失败"
            if request.status > 400:
                return False, "错误码：" + str(request.status)
            content = request.read()
            content = content.decode("gbk")
            # 检测html中的js变量
            re_time = re.search(r"time='(\d*?) *?';", content, re.U)
            if re_time is None:
                return False, "没有登录网关，未检测到时间"
            re_flow = re.search(r"flow='(\d*?) *?';", content, re.U)
            if re_flow is None:
                return False, "没有登录网关，未检测到流量"
            re_fee = re.search(r"fee='(\d*?) *?';", content, re.U)
            if re_fee is None:
                return False, "没有登录网关，未检测到费用"
            html_time = re_time.group(1)
            html_flow = re_flow.group(1)
            html_fee = re_fee.group(1)
            html_flow1 = round(int(html_flow) // 1024 / 1024 * 100) / 100# 保留两位小数
            html_fee1 = (int(html_fee) // 100) / 100
            msg = "已使用时间" + html_time + "分钟，"
            msg += "已使用流量" + str(html_flow1) + "GB，"
            msg += "剩余金额" + str(html_fee1) + "元。"
            return True, msg
        except Exception as ex:
            return False, str(ex)

    def run(self):
        is_login = False
        while True:
            suc, msg = self.check()
            if suc is True:
                # 首次登录时输出信息
                if not is_login:
                    print(msg)
                is_login = True
                time.sleep(30)
            else:
                is_login = False
                print(msg)
                suc, msg = self.login()
                print(msg)
                if suc is False:
                    time.sleep(1)

    def start_monitor(self):
        """开始监控网关状态"""
        if not self.isAlive():
            self.start()

    def stop_monitor(self):
        """停止监控网关状态"""
        if self.isAlive():
            self.join()

    _account = {"account": "", "password": "", "type": 4}


if __name__ == "__main__":
    net = BjutNet()
    net.start_monitor()
