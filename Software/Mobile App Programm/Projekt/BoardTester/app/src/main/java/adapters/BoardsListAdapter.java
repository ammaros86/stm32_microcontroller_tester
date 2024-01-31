package adapters;

import static helpers.Constants.BOARD_NAME_KEY;
import static helpers.Constants.boardID;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.example.boardtester.R;

import java.util.ArrayList;

import boardTestProcesses.NewBoardTestActivity;
import database.AddRemoveBoardHandler;
import models.BoardTestCommands;

public class BoardsListAdapter extends RecyclerView.Adapter<BoardsListAdapter.MainViewHolder> {

    private final ArrayList<BoardTestCommands> boardList;
    private final Context context;
    private final AddRemoveBoardHandler boardsHandler;

    public BoardsListAdapter(Context context, AddRemoveBoardHandler boardsHandler, ArrayList<BoardTestCommands> boardList) {
        this.context = context;
        this.boardsHandler = boardsHandler;
        this.boardList = boardList;
    }

    @NonNull
    @Override
    public BoardsListAdapter.MainViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.row_boards, parent, false);
        return new BoardsListAdapter.MainViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull BoardsListAdapter.MainViewHolder holder, int position) {
        BoardTestCommands boardTestController = boardList.get(position);
        if(boardTestController != null){
            holder.boardIdTextView.setText(boardTestController.getBoardName());
            setListener(holder.relativeLayoutView, boardTestController, position, holder.removeBoardImageView);
        }
    }

    private void setListener(RelativeLayout relativeLayout, BoardTestCommands boardTestController, int position, ImageView removeBoard){
        relativeLayout.setOnClickListener(view -> {
            Intent intent = new Intent(context, NewBoardTestActivity.class);
            intent.putExtra(BOARD_NAME_KEY, boardTestController.getBoardName());
            intent.putExtra(boardID, boardTestController.getBoard_id());
            context.startActivity(intent);
        });
        removeBoard.setOnClickListener(view -> boardsHandler.removeBoard(boardTestController.getBoard_id(), position));
    }

    public void removeAt(int position) {
        boardList.remove(position);
        notifyItemRemoved(position);
        notifyItemRangeChanged(position, boardList.size());
    }

    @Override
    public int getItemCount() {
        return boardList.size();
    }

    public static class MainViewHolder extends RecyclerView.ViewHolder {

        private final TextView boardIdTextView;
        private final RelativeLayout relativeLayoutView;
        private final ImageView removeBoardImageView;

        MainViewHolder(View view) {
            super(view);
            relativeLayoutView = itemView.findViewById(R.id.relativeBoard);
            boardIdTextView = itemView.findViewById(R.id.boardIDText);
            removeBoardImageView = itemView.findViewById(R.id.removeBoard);
        }
    }
}

