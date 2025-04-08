package hust.cs.javacourse.search.query.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.query.AbstractHit;
import hust.cs.javacourse.search.query.Sort;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * PhraseSort类是Sort接口基于装饰者模式的实现。
 *              实现了短语（两个单词必须相邻）搜索的得分计算方式，
 */
public class PhraseSort implements Sort {
    private final Sort inputSorter;

    /**
     * 构造函数，接收一个Sort对象作为参数
     *
     * @param inputSorter : 被装饰的Sort对象
     */

    public PhraseSort(Sort inputSorter) {
        this.inputSorter = inputSorter;
    }

    /**
     * 对搜索结果进行排序的方法。使用被装饰的Sort对象的排序方法。
     *
     * @param hits : 搜索结果列表
     */

    @Override
    public void sort(List<AbstractHit> hits) {
        inputSorter.sort(hits);
    }

    /**
     * 计算搜索结果的得分的方法。得分为相邻的查询词对的数量。
     *
     * @param hit : 搜索结果
     * @return      搜索结果的得分
     */

    @Override
    public double score(AbstractHit hit) {
        List<Map.Entry<AbstractTerm, AbstractPosting>> postings = new ArrayList<>(hit.getTermPostingMapping().entrySet());
        AbstractPosting posting1 = postings.get(0).getValue();
        AbstractPosting posting2 = postings.get(1).getValue();
        int ans = 0;
        for (int i = 0; i < posting1.getPositions().size(); i++) {
            int nowPosition = posting1.getPositions().get(i);
            if (posting2.getPositions().contains(nowPosition + 1)) {
                ans++;
            }
        }
        return ans;
    }
}
