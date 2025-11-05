import java.util.*;

public class Assignment_7 {
    static int N, R;
    static boolean[][] conflict;
    static int[] courseSize;
    static int[] roomCap;

    // ANSI colors
    static final String GREEN = "\u001B[32m";
    static final String YELLOW = "\u001B[33m";
    static final String RED = "\u001B[31m";
    static final String RESET = "\u001B[0m";

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print(YELLOW + "Number of courses (N): " + RESET);
        N = Integer.parseInt(sc.nextLine().trim());

        System.out.print(YELLOW + "Number of rooms (R): " + RESET);
        R = Integer.parseInt(sc.nextLine().trim());

        courseSize = new int[N];
        System.out.println(YELLOW + "Enter course sizes (one per line):" + RESET);
        for (int i = 0; i < N; ++i) courseSize[i] = Integer.parseInt(sc.nextLine().trim());

        roomCap = new int[R];
        System.out.println(YELLOW + "Enter room capacities (one per line):" + RESET);
        for (int i = 0; i < R; ++i) roomCap[i] = Integer.parseInt(sc.nextLine().trim());

        conflict = new boolean[N][N];
        System.out.print(YELLOW + "Number of students (S): " + RESET);
        int S = Integer.parseInt(sc.nextLine().trim());

        System.out.println(YELLOW + "Enter enrolled course IDs per student (space-separated, 0-based):" + RESET);
        for (int s = 0; s < S; ++s) {
            String line = sc.nextLine().trim();
            if (line.isEmpty()) {
                s--;
                continue;
            }
            String[] parts = line.split("\\s+");
            List<Integer> list = new ArrayList<>();
            for (String p : parts) list.add(Integer.parseInt(p));
            for (int i = 0; i < list.size(); ++i)
                for (int j = i + 1; j < list.size(); ++j) {
                    int a = list.get(i), b = list.get(j);
                    conflict[a][b] = conflict[b][a] = true;
                }
        }

        // Room feasibility validation
        for (int i = 0; i < N; ++i) {
            boolean ok = false;
            for (int r = 0; r < R; ++r)
                if (roomCap[r] >= courseSize[i]) ok = true;
            if (!ok) {
                System.out.println(RED + "Course " + i + " cannot fit in any room. Exiting." + RESET);
                sc.close();
                return;
            }
        }

        boolean solved = false;
        int[] color = new int[N];
        int[] roomAssigned = new int[N];

        for (int k = 1; k <= N && !solved; ++k) {
            Arrays.fill(color, -1);
            Arrays.fill(roomAssigned, -1);
            boolean[][] usedRoom = new boolean[k][R];

            if (backtrack(0, k, color, roomAssigned, usedRoom)) {
                System.out.println(GREEN + "\n✅ Schedule found with " + k + " time slots\n" + RESET);
                for (int i = 0; i < N; ++i) {
                    System.out.printf(
                        "Course %d  →  ⟦Slot %d⟧  ⟦Room %d⟧%n",
                        i, color[i] + 1, roomAssigned[i]
                    );
                }
                solved = true;
            }
        }

        if (!solved)
            System.out.println(RED + "No feasible schedule found up to " + N + " slots." + RESET);

        sc.close();
    }

    static boolean backtrack(int idx, int k, int[] color, int[] roomAssigned, boolean[][] usedRoom) {
        if (idx == N) return true;

        for (int c = 0; c < k; ++c) {
            boolean conflictColor = false;
            for (int v = 0; v < N; ++v)
                if (conflict[idx][v] && color[v] == c) {
                    conflictColor = true;
                    break;
                }
            if (conflictColor) continue;

            for (int r = 0; r < R; ++r) {
                if (usedRoom[c][r]) continue;
                if (roomCap[r] < courseSize[idx]) continue;

                color[idx] = c;
                roomAssigned[idx] = r;
                usedRoom[c][r] = true;

                if (backtrack(idx + 1, k, color, roomAssigned, usedRoom))
                    return true;

                color[idx] = -1;
                roomAssigned[idx] = -1;
                usedRoom[c][r] = false;
            }
        }
        return false;
    }
}
