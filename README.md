<h1>Comparativo das Variações do MergeSort</h1>

<p>Este projeto de comparativo de desempenho de diferentes variações do algoritmo de MergeSort. O objetivo deste estudo é analisar a performance de diferentes versões do MergeSort ao ordenar vetores de diferentes tamanhos. Abaixo, você encontrará uma explicação detalhada de cada versão implementada</p>
<ul>
  <h2>
    Variações abordadas:
  </h2>
  <li>
    MergeSort Recurtivo
  </li>
    <li>
    MergeSort Interativo
  </li>
  <li>
    MergeSort por Insercao(m)
  </li>
  <li>
    Merge Sort com Multiway Merge(k):
  </li>
</ul>
Os testes de desempenho foram realizados em um computador com as seguintes especifica-
ções: processador Intel Core i5 de 12ª geração, sistema operacional Ubuntu Linux 22.04
LTS, 16GB de memória RAM DDR4 e placa de vídeo Intel Iris Xe. As comparações entre
os algoritmos são feitas com base no tempo de execução e no uso de recursos computaci-
onais. A tabela a seguir mostra os tempos de execução para diferentes tamanhos de vetor para cada algoritmo
<br/>
<br/>
<table border="1">
  <thead>
    <tr>
      <th>Algoritmos</th>
      <th>Tamanho do vetor</th>
      <th>1000</th>
      <th>5000</th>
      <th>10000</th>
      <th>50000</th>
      <th>100000</th>
      <th>500000</th>
      <th>1000000</th>
      <th>5000000</th>
      <th>10000000</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>MergeSort Recursivo</td>
      <td>Tempo (segundos)</td>
      <td>0.000258</td>
      <td>0.001312</td>
      <td>0.002539</td>
      <td>0.013572</td>
      <td>0.030725</td>
      <td>0.104751</td>
      <td>0.195456</td>
      <td>1.023708</td>
      <td>2.111316</td>
    </tr>
    <tr>
      <td>MergeSort Inserção (10)</td>
      <td>Tempo (segundos)</td>
      <td>0.000163</td>
      <td>0.001005</td>
      <td>0.001846</td>
      <td>0.012436</td>
      <td>0.021302</td>
      <td>0.087559</td>
      <td>0.166443</td>
      <td>0.813007</td>
      <td>1.690932</td>
    </tr>
    <tr>
      <td>MergeSort Inserção (100)</td>
      <td>Tempo (segundos)</td>
      <td>0.000129</td>
      <td>0.000880</td>
      <td>0.002034</td>
      <td>0.013519</td>
      <td>0.025876</td>
      <td>0.080797</td>
      <td>0.153973</td>
      <td>0.848784</td>
      <td>1.743604</td>
    </tr>
    <tr>
      <td>MergeSort Iterativo</td>
      <td>Tempo (segundos)</td>
      <td>0.000234</td>
      <td>0.001219</td>
      <td>0.002312</td>
      <td>0.012507</td>
      <td>0.022137</td>
      <td>0.083842</td>
      <td>0.164828</td>
      <td>0.877705</td>
      <td>1.852401</td>
    </tr>
    <tr>
      <td>MergeSort Multiway (10)</td>
      <td>Tempo (segundos)</td>
      <td>0.000196</td>
      <td>0.001258</td>
      <td>0.002422</td>
      <td>0.014586</td>
      <td>0.020402</td>
      <td>0.105253</td>
      <td>0.159571</td>
      <td>0.968634</td>
      <td>1.678090</td>
    </tr>
    <tr>
      <td>MergeSort Multiway (5)</td>
      <td>Tempo (segundos)</td>
      <td>0.000278</td>
      <td>0.001320</td>
      <td>0.002504</td>
      <td>0.011970</td>
      <td>0.024440</td>
      <td>0.082108</td>
      <td>0.166206</td>
      <td>0.859729</td>
      <td>1.584498</td>
    </tr>
  </tbody>
</table>
<br/>
 O desempenho das variações do MergeSort demonstra carac-
terísticas distintas com base em seu funcionamento. O MergeSort Recursivo apresenta o
maior tempo médio em vetores grandes devido à sobrecarga de chamadas recursivas. Já
o MergeSort Inserção(10) é mais eficiente em vetores pequenos, pois utiliza o algoritmo
de inserção para ordenar subvetores de 10 elementos antes da mesclagem. O MergeSort
Inserção(100), apesar de seguir o mesmo princípio apresenta desempenho inferior ao In-
serção(10), A variação Interativa se destaca em vetores pequenos, por ter a ausência de
de chamadas recursivas, mas perde eficiência conforme o tamanho do vetor cresce. Por
fim, os métodos Multiway, embora mais lentos para vetores pequenos, mostram melhorias
significativas para vetores grandes, como o Multiway(5) apresentando uma leve vantagem
sobre o Multiway(10).
