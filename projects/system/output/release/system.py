import json
import matplotlib.pyplot as plt
import numpy             as np
import pandas            as pd

from borb.io.read.types                                       import Decimal
from borb.pdf.canvas.color.color                              import X11Color
from borb.pdf.canvas.geometry.rectangle                       import Rectangle
from borb.pdf.canvas.layout.image.chart                       import Chart
from borb.pdf.canvas.layout.image.image                       import Image
from borb.pdf.canvas.layout.layout_element                    import Alignment
from borb.pdf.canvas.layout.page_layout.multi_column_layout   import SingleColumnLayout
from borb.pdf.canvas.layout.page_layout.page_layout           import PageLayout
from borb.pdf.canvas.layout.text.paragraph                    import Paragraph
from borb.pdf.canvas.layout.table.fixed_column_width_table    import FixedColumnWidthTable
from borb.pdf.canvas.layout.table.flexible_column_width_table import FlexibleColumnWidthTable
from borb.pdf.canvas.layout.table.table                       import Table, TableCell
from borb.pdf.document                                        import Document
from borb.pdf.page.page                                       import Page
from borb.pdf.pdf                                             import PDF

from PIL import Image as PILImage

from binance.spot import Spot
from enum         import Enum, unique
from math         import floor

# =============================================================================

def make_deviations(reward, config):
    
    current_year_month = "0000/00"
    current_year       = 0
    
    times_m = []
    times_y = []
    
    for i in range(0, len(reward)):
        
        parts = reward["date"][i].split("/")

        year_month = (parts[0] + "/" + parts[1])
            
        if (year_month != current_year_month):

            current_year_month = year_month
            
            times_m.append(reward["time"][i])

        year = int(parts[0])
            
        if (year != current_year):

            current_year = year
            
            times_y.append(reward["time"][i])

    times_m.append(reward["time"][len(reward) - 1])
            
    if(reward["date"][len(reward) - 1].split("/")[0] ==
       reward["date"][len(reward) - 2].split("/")[0]):
        times_y.append(reward["time"][len(reward) - 1])
    
    transaction = config["transaction"]

    deviations_m = []
    
    for i in range (1, len(times_m)):

        reward_1 = reward["reward"][(reward[reward["time"] == times_m[i - 1]].index[0])]
        reward_2 = reward["reward"][(reward[reward["time"] == times_m[i    ]].index[0])]
            
        if(config["has_reinvestment"]):
            deviations_m.append((reward_2 - reward_1) / (transaction + reward_1)) 
        else:
            deviations_m.append((reward_2 - reward_1) / (transaction))
    
    deviations_y = []
    
    for i in range (1, len(times_y)):

        reward_1 = reward["reward"][(reward[reward["time"] == times_y[i - 1]].index[0])]
        reward_2 = reward["reward"][(reward[reward["time"] == times_y[i    ]].index[0])]
            
        if(config["has_reinvestment"]):
            deviations_y.append((reward_2 - reward_1) / (transaction + reward_1)) 
        else:
            deviations_y.append((reward_2 - reward_1) / (transaction))
    
    return deviations_m, deviations_y

# =============================================================================

def sharpe_coefficient(reward, config):

    r = 0.1 / 100
    
    deviations_m, _ = make_deviations(reward, config)
    
    return (np.mean(deviations_m) - r / 12) / np.std(deviations_m)

# =============================================================================

def sortino_coefficient(reward, config):
    
    r = 0.1 / 100
    
    deviations_m, _ = make_deviations(reward, config)
    
    selected_deviations_m = [k for k in deviations_m if k < r / 12]
    
    return ((np.mean(deviations_m) - r / 12) / (np.std(selected_deviations_m) *
        np.sqrt(len(selected_deviations_m) / len(deviations_m))))

# =============================================================================

def make_statictics_table(reward_HS, reward_BH, trades_HS, config):
    
    fields = ["Traded instrument",   "Backtesting period",   "Working timeframe",
              "Strategy version",    "Initial investments",  "Has reinvestment",   
              "Strategy profit",     "Total profitability",  "Maximum drawdown",
              "Benchmark B&H",       "Trading fee rate",     "Fixed stop-loss",
              
              "Longs quantity",      "Shorts quantity",      "Trades quantity",
              
              "Longs with profit",   "Shorts with profit",   "Trades with profit", 
              "Longs with loss",     "Shorts with loss",     "Trades with loss",

              "Longs total profit",  "Shorts total profit",  "Trades total profit", 
              "Longs total loss",    "Shorts total loss",    "Trades total loss",

              "Long average profit", "Short average profit", "Trade average profit",
              "Long average loss",   "Short average loss",   "Trade average loss",

              "Long minimum hold",   "Short minimum hold",   "Trade minimum hold",
              "Long average hold",   "Short average hold",   "Trade average hold",
              "Long maximum hold",   "Short maximum hold",   "Trade maximum hold",
            
              "Sharpe coefficient",  "Sortino coefficient",  "Kalmar coefficient"]
    
    l_positions_with_profit = len(trades_HS[(trades_HS["position"] == "L") & (trades_HS["reward"] > 0)].index)
    s_positions_with_profit = len(trades_HS[(trades_HS["position"] == "S") & (trades_HS["reward"] > 0)].index)
    
    a_positions_with_profit = l_positions_with_profit + s_positions_with_profit
    
    l_positions_with_loss = len(trades_HS[(trades_HS["position"] == "L") & (trades_HS["reward"] < 0)].index)
    s_positions_with_loss = len(trades_HS[(trades_HS["position"] == "S") & (trades_HS["reward"] < 0)].index)
    
    a_positions_with_loss = l_positions_with_loss + s_positions_with_loss

    l_positions_in_total = l_positions_with_profit + l_positions_with_loss
    s_positions_in_total = s_positions_with_profit + s_positions_with_loss

    a_positions_in_total = l_positions_in_total + s_positions_in_total

    l_positions_total_profit = sum(trades_HS["reward"][trades_HS[(trades_HS["position"] == "L") & (trades_HS["reward"] > 0)].index])
    s_positions_total_profit = sum(trades_HS["reward"][trades_HS[(trades_HS["position"] == "S") & (trades_HS["reward"] > 0)].index])

    a_positions_total_profit = l_positions_total_profit + s_positions_total_profit
    
    l_positions_total_loss = sum(trades_HS["reward"][trades_HS[(trades_HS["position"] == "L") & (trades_HS["reward"] < 0)].index])
    s_positions_total_loss = sum(trades_HS["reward"][trades_HS[(trades_HS["position"] == "S") & (trades_HS["reward"] < 0)].index])

    a_positions_total_loss = l_positions_total_loss + s_positions_total_loss

    l_positions_average_profit = l_positions_total_profit / l_positions_with_profit
    s_positions_average_profit = s_positions_total_profit / s_positions_with_profit

    a_positions_average_profit = (l_positions_average_profit + s_positions_average_profit) / 2

    l_positions_average_loss = l_positions_total_loss / l_positions_with_loss
    s_positions_average_loss = s_positions_total_loss / s_positions_with_loss

    a_positions_average_loss = (l_positions_average_loss + s_positions_average_loss) / 2

    l_positions_minimum_hold = min(trades_HS["time_close"][trades_HS[trades_HS["position"] == "L"].index] -
                                   trades_HS["time_open" ][trades_HS[trades_HS["position"] == "L"].index]) / 3600.0
    s_positions_minimum_hold = min(trades_HS["time_close"][trades_HS[trades_HS["position"] == "S"].index] -
                                   trades_HS["time_open" ][trades_HS[trades_HS["position"] == "S"].index]) / 3600.0

    a_positions_minimum_hold = min(l_positions_minimum_hold, s_positions_minimum_hold)

    l_positions_average_hold = (sum(trades_HS["time_close"][trades_HS[trades_HS["position"] == "L"].index]) -
                                sum(trades_HS["time_open" ][trades_HS[trades_HS["position"] == "L"].index])) / l_positions_in_total / 3600.0
    s_positions_average_hold = (sum(trades_HS["time_close"][trades_HS[trades_HS["position"] == "S"].index]) -
                                sum(trades_HS["time_open" ][trades_HS[trades_HS["position"] == "S"].index])) / s_positions_in_total / 3600.0

    a_positions_average_hold = (l_positions_average_hold + s_positions_average_hold) / 2

    l_positions_maximum_hold = max(trades_HS["time_close"][trades_HS[trades_HS["position"] == "L"].index] -
                                   trades_HS["time_open" ][trades_HS[trades_HS["position"] == "L"].index]) / 3600.0
    s_positions_maximum_hold = max(trades_HS["time_close"][trades_HS[trades_HS["position"] == "S"].index] -
                                   trades_HS["time_open" ][trades_HS[trades_HS["position"] == "S"].index]) / 3600.0

    a_positions_maximum_hold = max(l_positions_maximum_hold, s_positions_maximum_hold)
    
    table = dict()

    table["Traded instrument"    ] = config["inputs_asset"].upper()
    table["Backtesting period"   ] = str(config["inputs_year_begin"]) +  " - " + str(config["inputs_year_end"])
    table["Working timeframe"    ] = str(config["inputs_timeframe"]) + config["inputs_timeframe_type"].upper()
    table["Strategy version"     ] = config["test_hard_strategy"]
    table["Initial investments"  ] = ("%.2f" % config["transaction"])
    table["Has reinvestment"     ] = str(config["has_reinvestment"])
    table["Strategy profit"      ] = ("%.2f" % reward_HS["reward"][len(reward_HS) - 1])
    table["Total profitability"  ] = ("%.2f" % (reward_HS["reward"][len(reward_HS) - 1] / config["transaction"] * 100)) + " %"
    table["Maximum drawdown"     ] = "undefined"
    table["Benchmark B&H"        ] = ("%.2f" % reward_BH["reward"][len(reward_BH) - 1])
    table["Trading fee rate"     ] = str(config["commission"] * 100) + " %"
    table["Fixed stop-loss"      ] = str(config["stop_loss"] * 100) + " %"
    
    table["Longs quantity"       ] = str(l_positions_in_total)
    table["Shorts quantity"      ] = str(s_positions_in_total)
    table["Trades quantity"      ] = str(a_positions_in_total)

    table["Longs with profit"    ] = str(l_positions_with_profit)
    table["Shorts with profit"   ] = str(s_positions_with_profit)
    table["Trades with profit"   ] = str(a_positions_with_profit)

    table["Longs with loss"      ] = str(l_positions_with_loss)
    table["Shorts with loss"     ] = str(s_positions_with_loss)
    table["Trades with loss"     ] = str(a_positions_with_loss)

    table["Longs total profit"   ] = ("%.2f" % (l_positions_total_profit))
    table["Shorts total profit"  ] = ("%.2f" % (s_positions_total_profit))
    table["Trades total profit"  ] = ("%.2f" % (a_positions_total_profit))

    table["Longs total loss"     ] = ("%.2f" % abs(l_positions_total_loss))
    table["Shorts total loss"    ] = ("%.2f" % abs(s_positions_total_loss))
    table["Trades total loss"    ] = ("%.2f" % abs(a_positions_total_loss))

    table["Long average profit" ] = ("%.2f" % l_positions_average_profit)
    table["Short average profit"] = ("%.2f" % s_positions_average_profit)
    table["Trade average profit"] = ("%.2f" % a_positions_average_profit)

    table["Long average loss"   ] = ("%.2f" % abs(l_positions_average_loss))
    table["Short average loss"  ] = ("%.2f" % abs(s_positions_average_loss))
    table["Trade average loss"  ] = ("%.2f" % abs(a_positions_average_loss))

    table["Long minimum hold"   ] = ("%.2f" % l_positions_minimum_hold) +  " hours"
    table["Short minimum hold"  ] = ("%.2f" % s_positions_minimum_hold) +  " hours"
    table["Trade minimum hold"  ] = ("%.2f" % a_positions_minimum_hold) +  " hours"

    table["Long average hold"   ] = ("%.2f" % l_positions_average_hold) +  " hours"
    table["Short average hold"  ] = ("%.2f" % s_positions_average_hold) +  " hours"
    table["Trade average hold"  ] = ("%.2f" % a_positions_average_hold) +  " hours"

    table["Long maximum hold"   ] = ("%.2f" % l_positions_maximum_hold) +  " hours"
    table["Short maximum hold"  ] = ("%.2f" % s_positions_maximum_hold) +  " hours"
    table["Trade maximum hold"  ] = ("%.2f" % a_positions_maximum_hold) +  " hours"
    
    table["Sharpe coefficient"            ] = ("%.2f" % sharpe_coefficient (reward_HS, config))
    table["Sortino coefficient"           ] = ("%.2f" % sortino_coefficient(reward_HS, config))
    
    table["Kalmar coefficient"            ] = "undefined"    
    
    return fields, table

# =============================================================================

def make_deviations_table(reward, config):
    
    table = [" " for i in range(0, 14 * 5)]
    
    deviations_m, deviations_y = make_deviations(reward, config)

    parts = reward["date"][0].split("/")
   
    first_year =  int(parts[0])
    first_month = int(parts[1])
    
    i = 14 * (first_year - 2017) + first_month

    n = 0
    k = 0
    
    while (k < len(deviations_m)):
        
        if((i + 1) % 14 == 0):
            table[i] = ("{:+.2f}".format(deviations_y[n] * 100))
            n += 1   
        elif(i % 14 == 0):
            table[i]= " "  
        else:
            table[i] = ("{:+.2f}".format(deviations_m[k] * 100))
            k += 1 

        i += 1
        
    table[-1] = ("{:+.2f}".format(deviations_y[-1] * 100))
                                
    year = 2017
    
    for i in range(0, 5):
        
        table[14 * i] = year
        year += 1
    
    return [" ", "Jan.", "Feb.", "Mar.", "Apr.", "May", "Jun.", "Jul.",
            "Aug.", "Sep.", "Oct.", "Nov.", "Dec.", "Annual"] + table

# =============================================================================

def make_plot(reward_HS, reward_BH, config):
    
    current_year = 0

    dates = []
    times = []

    for i in range(0, len(reward_BH)):
        
        year = int(reward_BH["date"][i].split("/")[0])
        
        if (year != current_year):

            current_year = year
            
            dates.append(current_year)
            times.append(reward_BH["time"][i])
        
    dates.pop(0)
    times.pop(0)
    
    plt.plot(reward_HS["time"], reward_HS["reward"], color = "green")
    plt.plot(reward_BH["time"], reward_BH["reward"], color = "black")
    
    plt.legend(["Strategy \"" + config["test_hard_strategy"] + "\"", "Benchmark B&H"])
    
    #plt.xlabel("Year",   labelpad = 0.0 )
    #plt.ylabel("Profit", labelpad = 0.0 )
    
    plt.xticks(times, dates, fontsize = 8)
    plt.yticks(              fontsize = 8)
    
    plt.tick_params(axis = "x", direction = "in", which = "both")
    #plt.figure(figsize = (40, 30))
    
    plt.savefig("system/result/" + config["inputs_asset"] + "/reward.png", dpi = 400)

    plt.close()

# =============================================================================

def make_report(path_reward_HS, path_trades_HS, path_reward_BH, path_config):

    reward_HS = pd.read_csv(path_reward_HS, names = ["date", "time", "reward"])
    
    trades_HS = pd.read_csv(path_trades_HS, names = ["date_open", "date_close", "time_open",
                                                     "time_close", "position", "reward"])
    
    reward_BH = pd.read_csv(path_reward_BH, names = ["date", "time", "reward"])
    
    with open(path_config, "r", encoding = "utf-8 ") as file:
        config = json.load(file)

    document = Document()
    
    page = Page(width = Decimal(595), height = Decimal(842))
    
    document.append_page(page)

    paragraph_1 = Paragraph("Backtesting results of strategy \"" + config["test_hard_strategy"] + "\" for " +
        config["inputs_asset"].upper() + " in " + str(config["inputs_timeframe"]) +
        config["inputs_timeframe_type"].upper() + " timeframe",
        font = "Helvetica-Bold", font_size = Decimal(16.1))

    paragraph_2 = Paragraph("Monthly percentage changes in the volume of investments used for trading",
        font = "Helvetica-Bold", font_size = Decimal(16.1))

    paragraph_3 = Paragraph("Comparison of returns between strategy \"" + config["test_hard_strategy"] + "\" and benchmark B&H",
        font = "Helvetica-Bold", font_size = Decimal(16.0))
    
    fields, statistics_table = make_statictics_table(reward_HS, reward_BH, trades_HS, config)
    
    table_1 = FixedColumnWidthTable(number_of_columns = 6, number_of_rows = 15, margin_left = Decimal(0))
    
    for i in range (0, len(fields) // 3):
        
        for j in range(0, 3):
            
            table_1.add(Paragraph(str(fields[3 * i + j]), text_alignment = Alignment.LEFT,
                padding_left = Decimal(3), padding_bottom = Decimal(5), font_size = Decimal(9), font = "Helvetica-Bold"))
            table_1.add(Paragraph(str(statistics_table[fields[3 * i + j]]), text_alignment = Alignment.CENTERED, 
                padding_bottom = Decimal(3), padding_top = Decimal(0), font_size = Decimal(9), font = "Helvetica"))

    deviations_table = make_deviations_table(reward_HS, config)
    
    table_2 = FixedColumnWidthTable(number_of_columns = 14, number_of_rows = 6, margin_left = Decimal(0))
    
    for i in range (0, len(deviations_table)):
        
        table_2.add(Paragraph(str(deviations_table[i]), text_alignment = Alignment.RIGHT, 
            padding_right = Decimal(3), padding_bottom = Decimal(3), font_size = Decimal(9), font = "Helvetica"))

    make_plot(reward_HS, reward_BH, config)
    
    plot = PILImage.open("system/result/" + config["inputs_asset"] + "/reward.png")

    chart = Image(plot, width = Decimal(680), height = Decimal(511))

    chart._do_layout_without_padding(page, Rectangle(-28, 180, 500, 300))
     
    paragraph_1._do_layout_without_padding(page, Rectangle(10, 829, 750, 10))
    paragraph_2._do_layout_without_padding(page, Rectangle(10, 556, 750, 10))
    paragraph_3._do_layout_without_padding(page, Rectangle(10, 440, 750, 10))
    
    table_1._do_layout_without_padding(page, Rectangle(10, 509, 575, 300))
    table_2._do_layout_without_padding(page, Rectangle(10, 537, 575, 0))

    with open("system/result/" + config["inputs_asset"] + "/" + config["inputs_asset"] + ".pdf", "wb") as file:  
        PDF.dumps(file, document)

# =============================================================================

class ClientsPool:
    
    def __init__(self):
        self.clients = dict()

    def make_client(self, public_key: str, secret_key: str, recvWindow: int):
        self.clients[public_key] = Connector(public_key = public_key, secret_key = secret_key, recvWindow = recvWindow)

    def get_client(self, public_key: str):
        return self.clients[public_key]

clients = ClientsPool()

# =============================================================================

def custom_receive_window(function):
    
    def wrapper(*args, **kwargs):
        return function(*args, **kwargs, recvWindow = args[0].recvWindow)
    
    return wrapper

# =============================================================================

class Connector(Spot):

    def __init__(self, public_key = None, secret_key = None, recvWindow: int = 10000):
        self.recvWindow = recvWindow
    super().__init__(key = public_key, secret = secret_key)

    @staticmethod
    def _transform_kline(kline):
        
        headers = ["time_open",
                   "price_open",
                   "price_high",
                   "price_low",
                   "price_close",
                   "volume_base",
                   "time_close",
                   "volume_quote",
                   "volume_buy_base",
                   "volume_buy_quote"]
        
        kline = [float(entry) if '.' in str(entry) else entry for entry in kline]
        
        kline = dict(zip(headers, [item for item in kline if kline.index(item) != -4 and kline.index(item) != -1]))
        
        return kline

    @staticmethod
    def _transform_trade(trade):
        
        trade.pop("id")
        trade.pop("isBestMatch")
        
        trade = dict([(entry[0], float(entry[1])) if '.' in str(entry[1]) else entry for entry in trade.items()])
        
        return trade

    def get_klines(self, symbol: str, limit: str, interval: str = "4h") -> str:
        
        klines_as_lists = self.klines(symbol, interval, limit = int(limit))
        klines_as_dicts = [self._transform_kline(kline) for kline in klines_as_lists]
        
        return json.dumps(klines_as_dicts)

    def get_trades(self, symbol: str, n: int) -> str:

        trades_as_lists = self.trades(symbol, limit = n)
        trades_as_dicts = [self._transform_trade(trade) for trade in trades_as_lists]
        
        return json.dumps(trades_as_dicts)

    def get_symbol_volume(self, symbol: str):
        
        price = self.ticker_price(symbol = symbol)["price"]
        asset = symbol.replace("USDT", "")
        quantity = 0.0
        
        for balance in self.margin_account()["balances"]:
            if balance["asset"] == asset:
                quantity = float(balance["free"])
                
        return quantity * price

    def get_available_usdt(self):
        for balance in self.margin_account()["userAssets"]:
            if balance["asset"] == "USDT":
                return balance["free"]

    def close_long_position(self, symbol: str):
        
        asset = symbol.replace("USDT", "")
        quantity = 0.0
        
        for balance in self.margin_account()["userAssets"]:
            if asset == balance["asset"]:
                quantity = float(balance["free"])
                
        precision = self.compute_precision(symbol = symbol)
        quantity = floor(quantity / precision) * precision
        
        return self.new_margin_order(symbol = symbol, side = "SELL", type = "MARKET", quantity = quantity)

    def close_short_position(self, symbol: str):
        
        asset = symbol.replace("USDT", "")
        quantity = 0.0
        
        for balance in self.margin_account()["userAssets"]:
            if asset == balance["asset"]:
                quantity = float(balance["borrowed"])
                
        precision = self.compute_precision(symbol = symbol)
        quantity = floor(quantity / precision) * precision
        
        return self.new_margin_order(symbol = symbol, side = "BUY", type = "MARKET", quantity = quantity, sideEffectType = "AUTO_REPAY")

    def make_long_position(self, symbol: str, usdt: float):
        return self.new_margin_order(symbol = symbol, side = "BUY", type = "MARKET", quoteOrderQty = usdt)

    def make_short_position(self, symbol: str, usdt: float):
        return self.new_margin_order(symbol = symbol, side = "SELL", type = "MARKET", quoteOrderQty = usdt, sideEffectType = "MARGIN_BUY")

    def compute_precision(self, symbol: str):
        
        symbol_information = self.exchange_info(symbol = symbol)
        filter_information = symbol_information["symbols"][0]["filters"]
        
        for filter in filter_information:
            if filter["filterType"] == "LOT_SIZE":
                return float(filter["stepSize"])
            
        return 1.0

    def get_current_state(self, symbol: str):
        
        asset = symbol.replace("USDT", "")
        precision = self.compute_precision(symbol)
        
        for data in self.margin_account()["userAssets"]:
            
            if asset == data["asset"]:
                
                netAsset = float(data["netAsset"])
                
                if abs(netAsset) < precision:
                    return 'N'
                return 'L' if netAsset > 0.0 else 'S'

    def make_null_position(self, symbol: str):
        
        if self.get_current_state(symbol = symbol) == 'S':
            return self.close_short_position(symbol)
        elif self.get_current_state(symbol = symbol) == 'L':
            return self.close_long_position(symbol)

    def get_total_value(self):

        total_usdt = 0.0
        
        for balance in self.margin_account()["userAssets"]:
            
            net = float(balance["netAsset"])
            asset = balance["asset"]
            
            if net != 0.0:
                
                price = 1.0 if asset == "USDT" else float(self.ticker_price(symbol = asset + "USDT")["price"])
                total_usdt += price * net
                
        return total_usdt

    @custom_receive_window
    def margin_account(self, *args, **kwargs):
        return super().margin_account(*args, **kwargs)

    @custom_receive_window
    def new_margin_order(self, *args, **kwargs):
        return super().new_margin_order(*args, **kwargs)

dummy = Connector()

# =============================================================================

def make_client(public_key: str, secret_key: str, recvWindow: str = "10000"):
    return clients.make_client(public_key = public_key, secret_key = secret_key, recvWindow = int(recvWindow))

def get_client(public_key: str):
    return clients.get_client(public_key = public_key)

def get_klines(symbol: str, limit: str, interval: str = "4h") -> str:
    return dummy.get_klines(symbol = symbol.upper(), limit = limit, interval = interval)

def get_symbol_volume(public_key: str, symbol: str):
    return clients.get_client(public_key = public_key).get_symbol_volume(symbol = symbol.upper())

def account(public_key: str):
    return clients.get_client(public_key = public_key).account()

def margin_account(public_key: str):
    return clients.get_client(public_key = public_key).margin_account()

def get_current_state(public_key: str, symbol: str):
    return clients.get_client(public_key = public_key).get_current_state(symbol = symbol.upper())

def get_available_usdt(public_key: str):
    return clients.get_client(public_key = public_key).get_available_usdt()

def get_total_value(public_key: str):
    return clients.get_client(public_key = public_key).get_total_value()

def make_long_position(public_key: str, symbol: str, usdt: str):
    return clients.get_client(public_key = public_key).make_long_position(symbol = symbol.upper(), usdt = float(usdt))

def make_short_position(public_key: str, symbol: str, usdt: str):
    return clients.get_client(public_key = public_key).make_short_position(symbol = symbol.upper(), usdt = float(usdt))

def make_null_position(public_key: str, symbol: str):
    return clients.get_client(public_key = public_key).make_null_position(symbol = symbol.upper())

# =============================================================================
