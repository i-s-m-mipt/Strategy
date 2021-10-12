import json
import matplotlib.pyplot as plt
import numpy             as np
import pandas            as pd

from borb.io.read.types                                     import Decimal
from borb.pdf.canvas.color.color                            import X11Color
from borb.pdf.canvas.geometry.rectangle                     import Rectangle
from borb.pdf.canvas.layout.image.chart                     import Chart
from borb.pdf.canvas.layout.image.image                     import Image
from borb.pdf.canvas.layout.layout_element                  import Alignment
from borb.pdf.canvas.layout.page_layout.multi_column_layout import SingleColumnLayout
from borb.pdf.canvas.layout.page_layout.page_layout         import PageLayout
from borb.pdf.canvas.layout.text.paragraph                  import Paragraph
from borb.pdf.canvas.layout.table.fixed_column_width_table  import FixedColumnWidthTable
from borb.pdf.canvas.layout.table.table                     import Table, TableCell
from borb.pdf.document                                      import Document
from borb.pdf.page.page                                     import Page
from borb.pdf.pdf                                           import PDF

from PIL import Image as PILImage

def make_report(path_reward, path_trades, path_reward_BH, path_config):

    reward = pd.read_csv(path_reward, names = ['date_time',
                                               'date_time_UNIX',
                                               'reward'])
    
    trades = pd.read_csv(path_trades, names = ['date_time_open',
                                               'date_time_close',
                                               'date_time_open_UNIX',
                                               'date_time_close_UNIX',
                                               'position',
                                               'reward'])
    
    reward_BH = pd.read_csv(path_reward_BH, names = ['date_time',
                                                     'date_time_UNIX',
                                                     'reward'])

    unique_time      = []
    unique_time_UNIX = []

    time = '1000/00'

    for i in range(0, len(reward)):
        
        parts = reward['date_time'][i].split("/")
        perts_time = time.split("/")
        
        if (parts[0] + '/' + parts[1]) != (perts_time[0] + '/' + perts_time[1]):
            unique_time_UNIX.append(reward['date_time_UNIX'][i])
            unique_time.append((parts[1]) + "." + str(int(parts[0]) - 2000))
            time = reward['date_time'][i]
        
    unique_time = unique_time[1:]
    unique_time_UNIX = unique_time_UNIX[1:]

    plt.plot(reward['date_time_UNIX'][:], reward['reward'][:], color = 'green') 
    plt.plot(reward['date_time_UNIX'][:], reward_BH['reward'][:], color = 'black')

    plt.legend(['Our strategy', 'Buy & Hold'])

    plt.xlabel('Date',       labelpad = -40.0 )
    plt.ylabel('Profit ($)', labelpad = -50.0 )

    plt.xticks(unique_time_UNIX, unique_time, rotation = 90, fontsize = 8)
    plt.yticks(                                              fontsize = 8)

    plt.tick_params(axis='x', direction='in', which ='both')

    plt.savefig('reward',  bbox_inches='tight')

    plot = PILImage.open('reward.png')

    parameters = ['Strategy', 'Asset', 'Backstory',
              'Timeframe', 'Reinvestment', 'Commission', 
              'Init investment ($)', 'Total profit', 'Interest profit',
              ' ', 'Profit on the Buy & Hold strategy', ' ',
              
              'Number of long positions (L)', 'Number of short positions (S)',  'Total number of transactions',
              'Number of L profitable positions', 'Number of S profitable positions', 'Total number of profitable transactions', 
              'Number of L losing positions', 'Number of S losing positions', 'Total number of losing transactionss',
              
              'Average time spent in one L position', 'Average time spent in one S position', 'Average time spent in one position',
              'Maximum time spent in one position', 'Minimum time spent in one position', ' ',
            
              'Profit amount in L positions', 'Profit amount in S positions', 'Profit amount', 
              'Loss amount in L positions', 'Loss amount in S positions','Loss amount',
              
              'Average profit on profitable positions', 'Average loss on unprofitable positions', ' ',
              'Sharpe coefficient', 'Sortino coefficient', 'Kalmar coefficient']
             
    #Cоздание словаря (название в таблице - значение)
    output_table = dict()

    #Заполнение словаря данными из config и reward файлов
    with open(path_config, 'r', encoding='utf-8') as f:
        text = json.load(f)
        
    base_value = text['transaction']

    output_table['Strategy'] = text['test_strategy']
    output_table['Asset'] = text['inputs_asset']
    output_table['Timeframe'] = str(text['inputs_timeframe']) + ' ' + text['inputs_timeframe_type']
    output_table['Backstory'] = str(text['inputs_year_begin']) + ' - ' + str(text['inputs_year_end']) #+ ' year'
    output_table['Reinvestment'] = 'No' #предполагается, что в config файле появится поле соответсвующее поле
    output_table['Commission'] = str(text['commission'] * 100) + " %"
    output_table['Init investment ($)'] = str(base_value)  
    output_table['Total profit'] = str(reward['reward'][len(reward) - 1])
    output_table['Interest profit'] = ("%.2f" % (reward['reward'][len(reward) - 1] / base_value * 100)) + ' %'
    output_table['Profit on the Buy & Hold strategy'] = str(reward_BH['reward'][len(reward) - 1])
       
    #Заполнение словаря данными из trades файла
    l_number = 0
    s_number = 0

    l_time = 0
    s_time = 0

    l_plus_number = 0
    l_minus_number = 0
    s_plus_number = 0
    s_minus_number = 0

    l_profit = 0
    l_loss = 0
    s_profit = 0
    s_loss = 0

    for i in range(0, len(trades) - 1):
        if(trades['position'][i] == 'long'):
            l_number+=1
            l_time+= (trades['date_time_close_UNIX'][i] - trades['date_time_open_UNIX'][i])
            
            if(float(trades['reward'][i]) > 0):
                l_plus_number+=1
                l_profit += trades['reward'][i]
            if(float(trades['reward'][i]) < 0):
                l_minus_number+=1
                l_loss += abs(trades['reward'][i])
                
        if(trades['position'][i] == 'short'):
            s_number+=1
            s_time+= (trades['date_time_close_UNIX'][i] - trades['date_time_open_UNIX'][i])
            
            if(float(trades['reward'][i]) > 0):
                s_plus_number+=1
                s_profit += trades['reward'][i]
            if(float(trades['reward'][i]) < 0):
                s_minus_number+=1
                s_loss += abs(trades['reward'][i])

    output_table['Number of long positions (L)'] = str(l_number)
    output_table['Number of short positions (S)'] = str(s_number)
    output_table['Total number of transactions'] = str(l_number + s_number)

    output_table['Number of L profitable positions'] = str(l_plus_number)
    output_table['Number of S profitable positions'] = str(s_plus_number)
    output_table['Total number of profitable transactions'] = str(l_plus_number + s_plus_number)

    output_table['Number of L losing positions'] = str(l_minus_number)
    output_table['Number of S losing positions'] = str(s_minus_number)
    output_table['Total number of losing transactionss'] = str(l_minus_number + s_minus_number)

    output_table['Average time spent in one L position'] = ('%.2f' % (l_time / l_number / 3600.0)) + ' h'

    output_table['Average time spent in one S position'] = ('%.2f' % (s_time / s_number / 3600.0)) + ' h'
    output_table['Average time spent in one position'] = ('%.2f' % ((l_time + s_time) / (l_number + s_number) / 3600.0)) + ' h'

    output_table['Maximum time spent in one position'] =  ('%.2f' % ( max(trades['date_time_close_UNIX'][:-1] - trades['date_time_open_UNIX'][:-1]) / 3600.0)) + ' h'
    output_table['Minimum time spent in one position'] = ('%.2f' % ( min(trades['date_time_close_UNIX'][:-1] - trades['date_time_open_UNIX'][:-1]) / 3600.0)) + ' h'

    output_table['Profit amount in L positions'] = ('%.2f' % (l_profit))
    output_table['Profit amount in S positions'] = ('%.2f' % (s_profit))
    output_table['Profit amount'] = ('%.2f' % (l_profit + s_profit))

    output_table['Loss amount in L positions'] = ('%.2f' % (l_loss))
    output_table['Loss amount in S positions'] = ('%.2f' % (s_loss))
    output_table['Loss amount'] = ('%.2f' % (l_loss + s_loss))

    output_table['Average profit on profitable positions'] =  ('%.2f' % ((l_profit + s_profit) / (l_number + s_number)))
    output_table['Average loss on unprofitable positions'] =  ('%.2f' % ((l_loss + s_loss) / (l_number + s_number)))
    output_table[' '] = ' '

    month_table = []

    for i in range (1, len(unique_time_UNIX)):
        begin_of_month = base_value + reward['reward'][(reward[reward['date_time_UNIX'] == unique_time_UNIX[i -1]].index[0])]
        end_of_month = base_value + reward['reward'][(reward[reward['date_time_UNIX'] == unique_time_UNIX[i]].index[0])]
        w = (end_of_month - begin_of_month) / begin_of_month
        month_table.append(w)  
   
    r = 0.1 / 100
    small_month_table = [k for k in month_table if k < r / 12]
    sharpe_koef = (np.mean(month_table) - r / 12)/np.std(month_table)
    sortino_koef = (np.mean(month_table) - r / 12)/( (np.std(small_month_table)) * np.sqrt(len(small_month_table)/len(month_table)) )

    extremums = []

    for i in range(1, len(reward) - 1):
        if(   ((reward['reward'][i] > reward['reward'][i-1]) and (reward['reward'][i] > reward['reward'][i+1]))  
           or ((reward['reward'][i] < reward['reward'][i-1]) and (reward['reward'][i] < reward['reward'][i+1])) ):
            extremums.append(reward['reward'][i])  

    differences = [(extremums[i] - extremums[i + 1]) for i in range (0, len(extremums) - 1)]
    maximum_drawdown = (extremums[differences.index(max(differences))] - 
                        extremums[differences.index(max(differences)) + 1]) /(extremums[differences.index(max(differences))] 
                                                                              + base_value) 

    kalmar_koef  = reward['reward'][len(reward) - 1] / base_value / maximum_drawdown

    output_table['Sharpe coefficient'] = ('%.2f' % (sharpe_koef))
    output_table['Sortino coefficient'] = ('%.2f' % (sortino_koef))
    output_table['Kalmar coefficient'] = ('%.2f' % (kalmar_koef))

    doc = Document()  
    page = Page(width = Decimal(595), height = Decimal(842))  
    doc.append_page(page)

    #Название для pdf файла
    paragraph = Paragraph(output_table['Strategy'], font="Helvetica-Bold", font_size = Decimal(16))

    #Таблица со статистическими параметрами для pdf файла
    table = FixedColumnWidthTable(number_of_columns=6, number_of_rows=13, margin_left = Decimal(0))

    for i in range (0, len(parameters) // 3):
        table.add(Paragraph(str(parameters[3 * i]), text_alignment = Alignment.LEFT, 
                            padding_left = Decimal(3), padding_bottom = Decimal(2), font_size = Decimal(8), font = 'Helvetica-bold'))
        table.add(Paragraph(str(output_table[parameters[3 * i]]), text_alignment = Alignment.CENTERED, 
                            padding_left = Decimal(3), padding_bottom = Decimal(2), font_size = Decimal(8)))
        table.add(Paragraph(str(parameters[3 * i + 1]), text_alignment = Alignment.LEFT, 
                            padding_left = Decimal(3), padding_bottom = Decimal(2), font_size = Decimal(8), font = 'Helvetica-bold'))
        table.add(Paragraph(str(output_table[parameters[3 * i + 1]]), text_alignment = Alignment.CENTERED, 
                            padding_left = Decimal(3), padding_bottom = Decimal(2), font_size = Decimal(8)))
        table.add(Paragraph(str(parameters[3 * i + 2]), text_alignment = Alignment.LEFT, 
                            padding_left = Decimal(3), padding_bottom = Decimal(2), font_size = Decimal(8), font = 'Helvetica-bold'))
        table.add(Paragraph(str(output_table[parameters[3 * i + 2]]), text_alignment = Alignment.CENTERED, 
                            padding_left = Decimal(3), padding_bottom = Decimal(2), font_size = Decimal(8)))

    #Картинка для pdf файла
    chart = Image(plot, width=Decimal(381), height=Decimal(260))

    #Таблица с доходностью по месяцам для pdf файла
    table1 = FixedColumnWidthTable(number_of_columns=2, number_of_rows=len (month_table), margin_left = Decimal(0))

    for i in range (0, len (month_table)):
        table1.add(Paragraph(str(unique_time[i+1]), text_alignment = Alignment.LEFT, 
                            padding_left = Decimal(3), padding_bottom = Decimal(2), font_size = Decimal(8), font = 'Helvetica-bold'))
        table1.add(Paragraph(('%.2f' % (month_table[i] * 100)) + ' %', text_alignment = Alignment.CENTERED, 
                            padding_left = Decimal(3), padding_bottom = Decimal(2), font_size = Decimal(8)))
    


    paragraph._do_layout_without_padding(page, Rectangle(10, 800, 10, 10))
    chart._do_layout_without_padding(page, Rectangle(10, 10, 381, 260))
    table._do_layout_without_padding(page, Rectangle(10, 300, 575, 300))
    #table1._do_layout_without_padding(page, Rectangle(10, 700, 200, 200))

    with open("output.pdf", "wb") as out_file_handle:  
        PDF.dumps(out_file_handle, doc)
