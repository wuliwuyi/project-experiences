package hust.cs.javacourse.search.query.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractPostingList;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.query.AbstractHit;
import hust.cs.javacourse.search.query.AbstractIndexSearcher;
import hust.cs.javacourse.search.query.Sort;
import java.io.File;
import java.util.*;

/**
 * IndexPhraseSearcher类是AbstractIndexSearcher基于装饰者模式的实现。
 *              提供了根据一个或两个查询词来搜索索引（包括短语搜索），并返回排序后的命中结果的功能。

 */

public class IndexPhraseSearcher extends AbstractIndexSearcher {

    private final AbstractIndexSearcher searcherInput;

    /**
     * 构造函数，接收一个AbstractIndexSearcher对象作为参数
     *
     * @param searcherInput : 被装饰的AbstractIndexSearcher对象
     */

    public IndexPhraseSearcher(AbstractIndexSearcher searcherInput) {
        this.searcherInput = searcherInput;
    }

    /**
     * 打开索引文件并加载索引。
     *
     * @param indexFile : 索引文件的路径
     */

    @Override
    public void open(String indexFile) {
        index.load(new File(indexFile));
        searcherInput.open(indexFile);
    }

    /**
     * 根据一个查询词搜索索引，并返回排序后的命中结果。
     *
     * @param queryTerm : 查询词
     * @param sorter    : 排序器
     * @return            排序后的命中结果
     */

    @Override
    public AbstractHit[] search(AbstractTerm queryTerm, Sort sorter) {
        return searcherInput.search(queryTerm, sorter);
    }

    /**
     * 根据两个查询词和一个逻辑组合方式搜索索引，并返回排序后的命中结果。
     * 特别的，当逻辑组合方式为AND时，实现了短语搜索，即两个查询词在文档中的位置必须是相邻的。
     *
     * @param queryTerm1 : 第一个查询词
     * @param queryTerm2 : 第二个查询词
     * @param sorter     : 排序器
     * @param combine    : 逻辑组合方式
     * @return             排序后的命中结果
     */

    @Override
    public AbstractHit[] search(AbstractTerm queryTerm1, AbstractTerm queryTerm2, Sort sorter, LogicalCombination combine) {
        if (combine == LogicalCombination.OR) {
            return null;
        }
        AbstractPostingList postingList1 = index.search(queryTerm1);
        AbstractPostingList postingList2 = index.search(queryTerm2);
        if (postingList1 == null || postingList2 == null) {
            return new AbstractHit[0];
        }
        Map<Integer, Map<AbstractTerm, AbstractPosting>> result = new HashMap<>();
        int i = 0, j = 0;
        while (i < postingList1.size() && j < postingList2.size()) {
            AbstractPosting posting1 = postingList1.get(i);
            AbstractPosting posting2 = postingList2.get(j);
            if (posting1.getDocId() == posting2.getDocId()) {
                List<Integer> positions1 = posting1.getPositions();
                List<Integer> positions2 = posting2.getPositions();
                for (int pos1 : positions1) {
                    if (positions2.contains(pos1 + 1)) {
                        Map<AbstractTerm, AbstractPosting> termPostingMapping = new HashMap<>();
                        termPostingMapping.put(queryTerm1, posting1);
                        termPostingMapping.put(queryTerm2, posting2);
                        result.put(posting1.getDocId(), termPostingMapping);
                        break;
                    }
                }
                i++;
                j++;
            } else if (posting1.getDocId() < posting2.getDocId()) {
                i++;
            } else {
                j++;
            }
        }

        sorter = new PhraseSort(sorter);
        if (result.isEmpty()) {
            return new AbstractHit[0];
        }
        AbstractHit[] hits = new AbstractHit[result.size()];
        List<Map.Entry<Integer, Map<AbstractTerm, AbstractPosting>>> entryList = new ArrayList<>(result.entrySet());
        for (int k = 0; k < hits.length; k++) {
            Map.Entry<Integer, Map<AbstractTerm, AbstractPosting>> entry = entryList.get(k);
            hits[k] = new Hit(entry.getKey(), index.getDocName(entry.getKey()), entry.getValue());
            hits[k].setScore(sorter.score(hits[k]));
        }
        List<AbstractHit> hitList = Arrays.asList(hits);
        sorter.sort(hitList);
        hits = hitList.toArray(hits);
        return hits;

    }

}